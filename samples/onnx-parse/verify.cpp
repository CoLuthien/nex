
#include "verify.hpp"

#include "nex/frontend/layer-description.hpp"
#include "nex/frontend/layer-schema.hpp"

#include "cmn/dispatch.hpp"
#include "cmn/encoded/encoded.hpp"
#include "cmn/floats/bf16.hpp"
#include "cmn/floats/f16.hpp"
#include "cmn/layout.hpp"
#include "cmn/numeric-types.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <map>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace sample
{
namespace
{

using namespace lnpu;
using nex::EArity;
using nex::layer_description;
using nex::layer_schema;

/// Beyond a handful, findings of the same shape say nothing more than the count does.
constexpr std::size_t max_findings = 12;

/**
 * @brief One stored element as a real number, whatever it is stored as.
 *
 * The fixed-point encodings have no real value without their scale, so what comes back for them
 * is the code itself -- which is what the checks here want anyway.
 */
template <typename T>
struct as_real
{
    static void process(encoded const& region, std::size_t index, double& into)
    {
        auto const value = region.values<T>()[index];

        if constexpr (one_of<T, f16, bf16>)
        {
            into = detail::as_f32(value);
        }
        else if constexpr (is_fixed_point_v<T>)
        {
            into = static_cast<double>(value.value);
        }
        else
        {
            into = static_cast<double>(value);
        }
    }
};

double
element_at(encoded const& region, std::size_t index)
{
    double value = 0.0;
    dispatch<as_real>(region.dtype(), region, index, value);

    return value;
}

bool
is_real_encoding(EDataType type)
{
    return type == EDataType::f32 or type == EDataType::f16 or type == EDataType::bf16;
}

/**
 * @brief Hands @p look at most @p sampled elements of @p region, spread evenly, and counts them.
 *
 * A weight of a language model is hundreds of megabytes and a graph has hundreds of them, so
 * reading every element is a choice rather than the default -- @p sampled of 0 is what makes it.
 * What is read is spread rather than taken from the front: an external weight handed out as the
 * wrong window of a mapped file looks perfectly ordinary for its first few elements, and the last
 * element is where a mapping that runs short shows itself, so it is always among them.
 *
 * @param look called with each position; returns false to stop the walk.
 */
template <typename Fn>
std::size_t
walk_over(encoded const& region, std::size_t sampled, Fn&& look)
{
    auto const count = region.n_element();
    if (count == 0)
    {
        return 0;
    }

    auto const taken = (sampled == 0 or sampled >= count) ? count : sampled;
    auto const step  = count / taken;

    std::size_t seen = 0;
    for (std::size_t which = 0; which < taken; ++which)
    {
        auto const position = (which + 1 == taken) ? count - 1 : which * step;
        ++seen;

        if (not look(position))
        {
            break;
        }
    }

    return seen;
}

/// "[1, 128, 2048]" -- layout::to_string() says a great deal more than a report line has room for.
std::string
shape_text(layout const& shape)
{
    std::string written = "[";
    for (layout::rank_type dim = 0; dim < shape.rank(); ++dim)
    {
        written += (dim > 0 ? ", " : "") + std::to_string(shape.extent(dim));
    }

    return written + "]";
}

std::string
shape_of(encoded const& region)
{
    return shape_text(region.layout());
}

std::string
describe(value_description const& value)
{
    std::string described = value.dtype ? to_string(*value.dtype) : std::string{"?"};
    described += value.layout ? shape_text(*value.layout) : std::string{"[?]"};

    if (value.quant)
    {
        described += " q" + std::to_string(static_cast<int>(value.quant->bitwidth));
        described += value.quant->axis ? "/axis" + std::to_string(*value.quant->axis) : "/tensor";
        described += value.quant->zero_point ? " asym" : " sym";
    }

    return described;
}

/// The value table has no size of its own; it ends where description(index) does.
std::size_t
value_count(network const& graph)
{
    std::size_t count = 0;
    while (nullptr != graph.description(count))
    {
        ++count;
    }

    return count;
}

/// Which layer writes each value, by the position of that layer in the graph's own order.
std::unordered_map<std::string_view, std::size_t>
producers_of(network const& graph)
{
    std::unordered_map<std::string_view, std::size_t> producer{};

    auto const layers = graph.layers();
    for (std::size_t position = 0; position < layers.size(); ++position)
    {
        auto const& layer = *layers[position];
        for (std::size_t result = 0; result < layer.output_count(); ++result)
        {
            auto const name = layer.output_name(result);
            if (not name.empty())
            {
                producer.emplace(name, position);
            }
        }
    }

    return producer;
}

/**
 * @brief How many elements one step along @p axis skips in a C-order tensor.
 *
 * Which channel an element belongs to is what a per-channel scale is looked up by, and that is
 * this number and the extent of the axis.
 */
std::size_t
inner_extent(layout const& shape, std::size_t axis)
{
    std::size_t inner = 1;
    for (auto dim = axis + 1; dim < shape.rank(); ++dim)
    {
        inner *= static_cast<std::size_t>(shape.extent(dim));
    }

    return inner;
}

void
add_finding(verdict& into, std::string finding)
{
    if (into.findings.size() < max_findings)
    {
        into.note(std::move(finding));
    }
    else if (into.findings.size() == max_findings)
    {
        into.note("... more of the same, not listed");
    }
}

std::string
name_of(layer_description const& layer)
{
    return std::string{layer.name()} + " (" + std::string{layer.op_type()} + ")";
}

/**
 * @brief Reads one quantization the way anything downstream would have to.
 *
 * @param owned_shape the shape of what carries the parameters, when it is known. An activation
 *        has none until inference runs, so the per-channel extent cannot be checked there.
 */
void
inspect(quantization const& quant,
        std::string const&  owner,
        layout const*       owned_shape,
        verdict&            into)
{
    if (not quant.scale)
    {
        add_finding(into, owner + ": quantized, but carries no scale");
        return;
    }

    if (quant.scale->dtype() != EDataType::f32)
    {
        add_finding(into,
                    owner + ": scale is " + to_string(quant.scale->dtype()) +
                        ", which nothing downstream reads as one");
    }

    if (quant.bitwidth == 0)
    {
        add_finding(into, owner + ": a bitwidth of zero says nothing about the code range");
    }

    auto const scales = quant.scale->n_element();
    if (scales == 0)
    {
        add_finding(into, owner + ": an empty scale");
        return;
    }

    for (std::size_t at = 0; at < scales; ++at)
    {
        auto const scale = element_at(*quant.scale, at);
        if (not std::isfinite(scale) or scale <= 0.0)
        {
            add_finding(into,
                        owner + ": scale[" + std::to_string(at) + "] is " + std::to_string(scale) +
                            ", which no value can be recovered through");
            break;
        }
    }

    if (quant.zero_point)
    {
        if (quant.zero_point->dtype() != EDataType::i32)
        {
            add_finding(into,
                        owner + ": zero point is " + to_string(quant.zero_point->dtype()) +
                            ", not i32");
        }

        if (quant.zero_point->n_element() != scales)
        {
            add_finding(into,
                        owner + ": " + std::to_string(quant.zero_point->n_element()) +
                            " zero points against " + std::to_string(scales) + " scales");
        }
    }

    if (not quant.axis and scales != 1)
    {
        add_finding(into,
                    owner + ": " + std::to_string(scales) +
                        " scales without an axis to spend them along");
    }

    if (quant.axis and nullptr != owned_shape)
    {
        if (*quant.axis >= owned_shape->rank())
        {
            add_finding(into,
                        owner + ": quantized along axis " + std::to_string(*quant.axis) + " of a " +
                            std::to_string(owned_shape->rank()) + "-d tensor");
        }
        else if (static_cast<std::size_t>(owned_shape->extent(*quant.axis)) != scales)
        {
            add_finding(into,
                        owner + ": " + std::to_string(scales) + " scales along an axis of " +
                            std::to_string(owned_shape->extent(*quant.axis)) + " channels");
        }
    }
}

} // namespace

void
summarise(network const& graph)
{
    auto const layers = graph.layers();

    std::map<std::string, std::size_t> histogram{};
    std::unordered_set<std::string>    weights_seen{};

    std::size_t weight_bytes = 0;
    std::size_t unsigned_ops = 0; // layers the registry had no signature for

    for (auto const& layer : layers)
    {
        histogram[std::string{layer->op_type()}] += 1;
        if (nullptr == layer->schema())
        {
            ++unsigned_ops;
        }

        for (std::size_t at = 0; at < layer->parameter_count(); ++at)
        {
            auto const weight = layer->parameter(at);
            if (weight and weights_seen.emplace(weight->fmt().name).second)
            {
                weight_bytes += weight->n_bytes();
            }
        }
    }

    auto const  values    = value_count(graph);
    std::size_t quantized = 0;
    for (std::size_t at = 0; at < values; ++at)
    {
        if (graph.description(at)->quant)
        {
            ++quantized;
        }
    }

    std::printf("\n-- graph --\n");
    std::printf("layers     : %zu, over %zu operators (%zu without a signature)\n",
                layers.size(),
                histogram.size(),
                unsigned_ops);
    std::printf(
        "values     : %zu, of which %zu carry quantization parameters\n", values, quantized);
    std::printf("weights    : %zu tensors, %.1f MiB\n",
                weights_seen.size(),
                static_cast<double>(weight_bytes) / (1024.0 * 1024.0));

    for (std::size_t at = 0; at < graph.n_inputs(); ++at)
    {
        auto const* described = graph.input_description(at);
        std::printf("input  [%zu] : %s %s\n",
                    at,
                    described ? described->name.c_str() : "<missing>",
                    described ? describe(*described).c_str() : "");
    }

    for (std::size_t at = 0; at < graph.n_outputs(); ++at)
    {
        auto const* described = graph.output_description(at);
        std::printf("output [%zu] : %s %s\n",
                    at,
                    described ? described->name.c_str() : "<missing>",
                    described ? describe(*described).c_str() : "");
    }

    // most common operators first; the tail is a long list of one-offs and says less
    std::vector<std::pair<std::string, std::size_t>> ranked{histogram.begin(), histogram.end()};
    std::sort(ranked.begin(), ranked.end(), [](auto const& lhs, auto const& rhs) {
        return lhs.second > rhs.second;
    });

    std::printf("operators  :");
    for (std::size_t at = 0; at < ranked.size() and at < 12; ++at)
    {
        std::printf(" %s x%zu", ranked[at].first.c_str(), ranked[at].second);
    }
    std::printf("%s\n", ranked.size() > 12 ? " ..." : "");
}

void
list_layers(network const& graph, std::size_t count)
{
    auto const layers = graph.layers();
    if (count == 0 or layers.empty())
    {
        return;
    }

    std::printf("\n-- first %zu layers --\n", std::min(count, layers.size()));

    for (std::size_t at = 0; at < layers.size() and at < count; ++at)
    {
        auto const& layer = *layers[at];
        std::printf("[%3zu] %s : %s\n",
                    at,
                    std::string{layer.name()}.c_str(),
                    std::string{layer.op_type()}.c_str());

        for (std::size_t operand = 0; operand < layer.input_count(); ++operand)
        {
            auto const name = layer.input_name(operand);
            if (name.empty())
            {
                std::printf("        in  <omitted>\n");
                continue;
            }

            auto const weight = layer.parameter(name);
            std::printf("        in  %s : %s%s\n",
                        std::string{name}.c_str(),
                        describe(*layer.input(operand)).c_str(),
                        weight ? " [weight]" : "");
        }

        for (std::size_t result = 0; result < layer.output_count(); ++result)
        {
            std::printf("        out %s : %s\n",
                        std::string{layer.output_name(result)}.c_str(),
                        describe(*layer.output(result)).c_str());
        }

        for (auto const& attr : layer.attributes())
        {
            std::printf("        @%s\n", attr.name.c_str());
        }
    }
}

verdict
check_interface(network const& graph)
{
    verdict result{.what = "graph interface"};

    if (graph.n_inputs() == 0)
    {
        result.note("the graph takes nothing in");
    }

    if (graph.n_outputs() == 0)
    {
        result.note("the graph hands nothing back");
    }

    auto const both = [&](char const* side, std::size_t at, value_description const* described) {
        ++result.examined;

        if (nullptr == described)
        {
            add_finding(result, std::string{side} + " " + std::to_string(at) + " is null");
            return;
        }

        if (described->name.empty())
        {
            add_finding(result, std::string{side} + " " + std::to_string(at) + " has no name");
            return;
        }

        if (graph.description(described->name) != described)
        {
            add_finding(result,
                        described->name + ": the " + side +
                            " the graph lists is not the one its value table hands out");
        }

        if (not described->dtype or not described->layout)
        {
            add_finding(result,
                        described->name + ": a graph " + side +
                            " with no type or no shape cannot be allocated");
        }
    };

    for (std::size_t at = 0; at < graph.n_inputs(); ++at)
    {
        both("input", at, graph.input_description(at));
    }

    for (std::size_t at = 0; at < graph.n_outputs(); ++at)
    {
        both("output", at, graph.output_description(at));
    }

    result.summary =
        std::to_string(graph.n_inputs()) + " in, " + std::to_string(graph.n_outputs()) + " out";

    return result;
}

verdict
check_value_table(network const& graph)
{
    verdict result{.what = "value table"};

    std::unordered_map<std::string_view, std::size_t> seen{};

    auto const count = value_count(graph);
    for (std::size_t at = 0; at < count; ++at)
    {
        auto const* described = graph.description(at);
        ++result.examined;

        if (described->name.empty())
        {
            add_finding(result, "the value at " + std::to_string(at) + " has no name");
            continue;
        }

        if (graph.description(described->name) != described)
        {
            add_finding(result,
                        described->name + ": looking it up by name lands on a different entry "
                                          "than looking it up by index");
        }

        auto const [entry, is_new] = seen.emplace(described->name, at);
        if (not is_new)
        {
            add_finding(result,
                        described->name + ": listed twice, at " + std::to_string(entry->second) +
                            " and " + std::to_string(at));
        }
    }

    result.summary = std::to_string(count) + " values, each reachable by index and by name";
    return result;
}

verdict
check_operands(network const& graph)
{
    verdict result{.what = "operand resolution"};

    std::size_t omitted = 0;

    for (auto const& held : graph.layers())
    {
        auto const& layer = *held;

        if (layer.name().empty() or layer.op_type().empty())
        {
            add_finding(result, "a layer with no name or no operator type");
        }

        auto const side = [&](char const* what, std::size_t at, bool is_operand) {
            auto const  name      = is_operand ? layer.input_name(at) : layer.output_name(at);
            auto const* described = is_operand ? layer.input(at) : layer.output(at);
            ++result.examined;

            if (nullptr == described)
            {
                add_finding(result,
                            name_of(layer) + ": null " + what + " at " + std::to_string(at));
                return;
            }

            if (name.empty())
            {
                // an operand the node left out still occupies its position; a result may not
                if (is_operand)
                {
                    ++omitted;
                }
                else
                {
                    add_finding(result,
                                name_of(layer) + ": result " + std::to_string(at) + " is unnamed");
                }
                return;
            }

            auto const* known = graph.description(name);
            if (nullptr == known)
            {
                add_finding(result,
                            name_of(layer) + ": its " + what + " '" + std::string{name} +
                                "' is in no value table");
                return;
            }

            // the layer holds its own copy of the description; a copy taken before the table was
            // finished would be missing what the table learned afterwards
            if (known->dtype != described->dtype or known->layout != described->layout or
                known->quant.has_value() != described->quant.has_value())
            {
                add_finding(result,
                            name_of(layer) + ": its view of '" + std::string{name} +
                                "' disagrees with the graph's");
            }

            if (is_operand and layer.input(name) == nullptr)
            {
                add_finding(result,
                            name_of(layer) + ": '" + std::string{name} +
                                "' is an operand that cannot be found by name");
            }
        };

        for (std::size_t at = 0; at < layer.input_count(); ++at)
        {
            side("operand", at, true);
        }

        for (std::size_t at = 0; at < layer.output_count(); ++at)
        {
            side("result", at, false);
        }
    }

    result.summary = std::to_string(result.examined) + " operands and results, " +
                     std::to_string(omitted) + " optional ones left out";

    return result;
}

verdict
check_dependency_order(network const& graph)
{
    verdict result{.what = "dependency order"};

    auto const layers   = graph.layers();
    auto const producer = producers_of(graph);

    std::unordered_set<std::string_view> written{};

    for (std::size_t position = 0; position < layers.size(); ++position)
    {
        auto const& layer = *layers[position];

        for (std::size_t at = 0; at < layer.input_count(); ++at)
        {
            auto const name = layer.input_name(at);
            if (name.empty())
            {
                continue;
            }

            ++result.examined;

            auto const found = producer.find(name);
            if (found == producer.end())
            {
                // written by nobody: a graph input or an initializer, both available from the
                // start. Anything else is a value read out of nowhere.
                if (nullptr == graph.parameter(name) and nullptr == graph.description(name))
                {
                    add_finding(result,
                                name_of(layer) + ": reads '" + std::string{name} +
                                    "', which nothing produces and no initializer holds");
                }
                continue;
            }

            if (found->second >= position)
            {
                add_finding(result,
                            name_of(layer) + ": reads '" + std::string{name} + "' at " +
                                std::to_string(position) + ", written at " +
                                std::to_string(found->second));
            }
        }

        for (std::size_t at = 0; at < layer.output_count(); ++at)
        {
            auto const name = layer.output_name(at);
            if (name.empty())
            {
                continue;
            }

            if (not written.emplace(name).second)
            {
                add_finding(result,
                            name_of(layer) + ": '" + std::string{name} +
                                "' is written by more than one layer");
            }
        }
    }

    result.summary =
        std::to_string(result.examined) + " reads, every one of them after the write it depends on";

    return result;
}

verdict
check_signatures(network const& graph)
{
    verdict result{.what = "operator signatures", .advisory = true};

    std::map<std::string, std::size_t> unknown{};

    for (auto const& held : graph.layers())
    {
        auto const& layer = *held;
        ++result.examined;

        auto const* schema = layer.schema();
        if (nullptr == schema)
        {
            unknown[std::string{layer.op_type()}] += 1;
            continue;
        }

        auto const fits =
            [&](std::vector<nex::operand_role> const& roles, std::size_t given, char const* what) {
                std::size_t required = 0;
                bool        variadic = false;

                for (auto const& role : roles)
                {
                    required += (role.arity == EArity::single) ? 1 : 0;
                    variadic = variadic or role.arity == EArity::variadic;
                }

                if (given < required or (not variadic and given > roles.size()))
                {
                    add_finding(result,
                                name_of(layer) + ": " + std::to_string(given) + " " + what +
                                    " against a signature that takes " + std::to_string(required) +
                                    " to " + std::to_string(roles.size()));
                }
            };

        fits(schema->operands, layer.input_count(), "operands");
        fits(schema->results, layer.output_count(), "results");
    }

    std::string missing{};
    for (auto const& [op_type, count] : unknown)
    {
        missing += " " + op_type + " x" + std::to_string(count);
    }

    result.summary = missing.empty()
                         ? "every layer resolved to a signature"
                         : "no signature for:" + missing +
                               " -- a backend has to know these operators by name alone";

    return result;
}

verdict
check_parameters(network const& graph, depth how_far)
{
    verdict result{.what = "weights"};

    std::unordered_set<std::string> seen{};
    std::size_t                     bytes = 0;
    std::size_t                     read  = 0;

    for (auto const& held : graph.layers())
    {
        auto const& layer = *held;

        for (std::size_t at = 0; at < layer.parameter_count(); ++at)
        {
            auto const weight = layer.parameter(at);
            ++result.examined;

            if (not weight)
            {
                add_finding(result,
                            name_of(layer) + ": parameter " + std::to_string(at) + " is null");
                continue;
            }

            auto const name = std::string{layer.parameter_name(at)};
            if (name.empty())
            {
                add_finding(result,
                            name_of(layer) + ": parameter " + std::to_string(at) + " has no name");
                continue;
            }

            // a weight is an initializer the node reads, so it has to be all three of these: the
            // layer's by name, the graph's by name, and one of the layer's own operands
            if (layer.parameter(name) != weight)
            {
                add_finding(result, name + ": the layer's index and name disagree about it");
            }

            if (graph.parameter(name) != weight)
            {
                add_finding(result,
                            name + ": the layer holds a different tensor than the graph does");
            }

            if (nullptr == layer.input(name))
            {
                add_finding(result, name + ": a weight of a layer that does not read it");
            }

            if (not seen.emplace(name).second)
            {
                continue; // shared between layers; already looked at
            }

            if (not has_fixed_element_size(weight->dtype()))
            {
                add_finding(result,
                            name + ": stored as " + to_string(weight->dtype()) +
                                ", which is not addressable as an array");
                continue;
            }

            auto const source = weight->source();
            if (not source or nullptr == weight->ptr())
            {
                add_finding(result, name + ": no storage behind it");
                continue;
            }

            if (weight->n_bytes() > source->size_bytes())
            {
                add_finding(result,
                            name + ": " + shape_of(*weight) + " needs " +
                                std::to_string(weight->n_bytes()) + " bytes of a " +
                                std::to_string(source->size_bytes()) + "-byte region");
                continue;
            }

            bytes += weight->n_bytes();

            // reading is what proves an external weight was mapped and windowed correctly: a
            // wrong window is either unreadable or full of values no weight ever holds
            auto const real = is_real_encoding(weight->dtype());

            read += walk_over(*weight, how_far.sampled, [&](std::size_t position) {
                if (real and not std::isfinite(element_at(*weight, position)))
                {
                    add_finding(result,
                                name + ": element " + std::to_string(position) + " of " +
                                    shape_of(*weight) + " is not a finite number");
                    return false;
                }

                return true;
            });
        }
    }

    result.summary = std::to_string(seen.size()) + " tensors, " +
                     std::to_string(bytes / (1024 * 1024)) + " MiB, " + std::to_string(read) +
                     " elements read back";

    return result;
}

verdict
check_quantization(network const& graph)
{
    verdict result{.what = "quantization parameters"};

    auto const  count     = value_count(graph);
    std::size_t quantized = 0;

    for (std::size_t at = 0; at < count; ++at)
    {
        auto const* described = graph.description(at);
        if (not described->quant)
        {
            continue;
        }

        ++quantized;
        ++result.examined;

        layout const* shape = described->layout ? &described->layout.value() : nullptr;
        inspect(*described->quant, described->name, shape, result);
    }

    std::unordered_set<std::string> seen{};
    std::size_t                     fake_quant = 0;

    for (auto const& held : graph.layers())
    {
        for (std::size_t at = 0; at < held->parameter_count(); ++at)
        {
            auto const weight = held->parameter(at);
            if (not weight or not weight->fmt().quant)
            {
                continue;
            }

            if (not seen.emplace(weight->fmt().name).second)
            {
                continue;
            }

            ++result.examined;
            fake_quant += weight->is_quantized() ? 0 : 1;

            auto const shape = weight->layout();
            inspect(*weight->fmt().quant, weight->fmt().name, &shape, result);
        }
    }

    result.summary = std::to_string(quantized) + " values and " + std::to_string(seen.size()) +
                     " weights carry parameters (" + std::to_string(fake_quant) +
                     " of the weights are still float, i.e. fake-quant metadata)";

    return result;
}

verdict
check_encodings_fit_weights(network const& graph, depth how_far)
{
    // Advisory: this reads the model, not the loader. A float weight an export already quantized
    // and dequantized sits on the grid its scale describes, and one that does not is either not
    // fake-quantized at all or paired with the encodings of a different export -- which nothing
    // else here would notice, since both files parse perfectly well either way.
    verdict result{.what = "encodings against the weights they describe", .advisory = true};

    std::unordered_set<std::string> seen{};

    std::size_t on_grid = 0; // tensors whose sampled values all sit on their own grid
    std::size_t adrift  = 0; // tensors whose values are not a multiple of their scale at all

    for (auto const& held : graph.layers())
    {
        for (std::size_t at = 0; at < held->parameter_count(); ++at)
        {
            auto const weight = held->parameter(at);
            if (not weight or not weight->fmt().quant or not is_real_encoding(weight->dtype()))
            {
                continue;
            }

            auto const& quant = *weight->fmt().quant;
            if (not quant.scale or quant.bitwidth == 0 or quant.bitwidth > 32)
            {
                continue; // check_quantization has already said so
            }

            auto const name = weight->fmt().name;
            if (not seen.emplace(name).second)
            {
                continue;
            }

            ++result.examined;

            auto const shape  = weight->layout();
            auto const scales = quant.scale->n_element();

            // which channel an element is quantized by; zero when one scale covers the tensor
            auto const axis  = quant.axis.value_or(0);
            auto const inner = (scales > 1 and axis < shape.rank()) ? inner_extent(shape, axis) : 0;

            // AIMET's mapping is defined over an unsigned code; dropping the zero point is what
            // shifts it down to the signed one the converter writes -- see
            // quantization-encoding.hpp
            auto const   symmetric = not quant.zero_point;
            double const highest   = symmetric
                                         ? static_cast<double>((1LL << (quant.bitwidth - 1)) - 1)
                                         : static_cast<double>((1LL << quant.bitwidth) - 1);
            double const lowest =
                symmetric ? -static_cast<double>(1LL << (quant.bitwidth - 1)) : 0.0;

            std::size_t outside = 0;
            std::size_t landed  = 0;
            std::size_t looked  = 0;
            double      worst   = 0.0;

            walk_over(*weight, how_far.sampled, [&](std::size_t position) {
                auto const channel = (inner == 0) ? 0 : (position / inner) % scales;
                auto const scale   = element_at(*quant.scale, std::min(channel, scales - 1));
                if (not std::isfinite(scale) or scale <= 0.0)
                {
                    return false; // check_quantization is the one that reports this
                }

                auto const zero_point =
                    quant.zero_point ? element_at(*quant.zero_point, std::min(channel, scales - 1))
                                     : 0.0;

                auto const code = element_at(*weight, position) / scale + zero_point;
                auto const step = std::abs(code - std::round(code));

                ++looked;
                worst = std::max(worst, step);

                // a rounding tolerance, not a fit: a code half a step out is no code at all
                landed += (step < 0.01) ? 1 : 0;
                outside += (code < lowest - 1.0 or code > highest + 1.0) ? 1 : 0;

                return true;
            });

            if (looked == 0)
            {
                continue;
            }

            if (landed == looked and outside == 0)
            {
                ++on_grid;
                continue;
            }

            ++adrift;

            char said[320];
            std::snprintf(said,
                          sizeof(said),
                          "%s: %zu of %zu sampled values are a multiple of its %d-bit scale "
                          "(worst is %.3f of a step out, %zu outside the code range)",
                          name.c_str(),
                          landed,
                          looked,
                          static_cast<int>(quant.bitwidth),
                          worst,
                          outside);

            add_finding(result, said);
        }
    }

    if (result.examined == 0)
    {
        result.summary = "no float weight carries an encoding to check against";
        return result;
    }

    result.summary = std::to_string(on_grid) + " of " + std::to_string(result.examined) +
                     " weights hold exactly the values their encoding can represent" +
                     (adrift == 0 ? ""
                                  : ", " + std::to_string(adrift) +
                                        " do not -- either the export left them in float or the "
                                        "encodings belong to another one");

    return result;
}

bool
conclude(std::vector<verdict> const& results)
{
    std::printf("\n-- checks --\n");

    bool sound = true;

    for (auto const& result : results)
    {
        char const* mark = result.passed() ? "ok  " : (result.advisory ? "note" : "FAIL");
        std::printf("%s %-42s %6zu examined\n", mark, result.what.c_str(), result.examined);

        if (not result.summary.empty())
        {
            std::printf("       %s\n", result.summary.c_str());
        }

        for (auto const& finding : result.findings)
        {
            std::printf("     - %s\n", finding.c_str());
        }

        sound = sound and (result.passed() or result.advisory);
    }

    std::printf("\n%s\n",
                sound ? "the graph came back whole: every check that decides this one passed"
                      : "the parse is not to be trusted; see the failures above");

    return sound;
}

} // namespace sample
