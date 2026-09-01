
#include "network-description.hpp"

#include "layer-description.hpp"
#include "onnx-decode.hpp"
#include "quantization-encoding.hpp"
#include "schema-registry.hpp"

#include "cmn/file.hpp"

#include <onnx.pb.h>
#include <spdlog/spdlog.h>

#include <cstddef>
#include <deque>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace lnpu::nex::nx
{
namespace
{

using name_list = decode::repeated<std::string>;

/// Both spellings of the standard operator set; "" is the one we key on.
std::string
canonical_domain(std::string_view domain)
{
    return (domain.empty() or domain == "ai.onnx") ? std::string{} : std::string{domain};
}

buffer::shared
map_whole(fs::path const& path)
{
    auto handle = file::load(path, 0);
    if (not handle)
    {
        throw std::runtime_error("[nx] cannot read '" + path.generic_string() + "'");
    }

    auto bytes = handle->map(0);
    if (not bytes)
    {
        throw std::runtime_error("[nx] cannot map '" + path.generic_string() + "'");
    }

    return bytes;
}

/**
 * @brief Puts the layers in an order where nothing is read before it is written.
 *
 * The onnx specification asks for the node list to be topologically sorted already, but exporters
 * do get this wrong and any later rewrite of the graph can undo it, so the order is established
 * here rather than trusted. A value no layer produces -- a graph input, an initializer -- is
 * available from the start and holds nobody up.
 *
 * @throws std::logic_error when the graph has a cycle, which leaves layers that never become
 *         ready.
 */
std::vector<layer_description::shared>
in_dependency_order(std::vector<layer_description::shared> const& layers)
{
    auto const count = layers.size();

    std::unordered_map<std::string_view, std::size_t> producer{};
    for (std::size_t position = 0; position < count; ++position)
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

    // for every layer, how many results of other layers it is still waiting for, and who is
    // waiting on it
    std::vector<std::size_t>              outstanding(count, 0);
    std::vector<std::vector<std::size_t>> waiting(count);

    for (std::size_t position = 0; position < count; ++position)
    {
        auto const& layer = *layers[position];
        for (std::size_t operand = 0; operand < layer.input_count(); ++operand)
        {
            auto const name = layer.input_name(operand);
            if (name.empty())
            {
                continue;
            }

            auto const found = producer.find(name);
            if (found == producer.end() or found->second == position)
            {
                continue;
            }

            waiting[found->second].emplace_back(position);
            ++outstanding[position];
        }
    }

    // the file order decides between layers that are ready at the same time, so the result is
    // the same on every run
    std::deque<std::size_t> ready{};
    for (std::size_t position = 0; position < count; ++position)
    {
        if (outstanding[position] == 0)
        {
            ready.emplace_back(position);
        }
    }

    std::vector<layer_description::shared> ordered{};
    ordered.reserve(count);

    while (not ready.empty())
    {
        auto const position = ready.front();
        ready.pop_front();

        ordered.emplace_back(layers[position]);

        for (auto const consumer : waiting[position])
        {
            if (--outstanding[consumer] == 0)
            {
                ready.emplace_back(consumer);
            }
        }
    }

    if (ordered.size() != count)
    {
        throw std::logic_error("[nx] the graph feeds " + std::to_string(count - ordered.size()) +
                               " layer(s) with their own output, directly or through others");
    }

    return ordered;
}

/**
 * @brief Turns a ModelProto into nx_network::contents.
 *
 * One instance reads one graph: the value table is filled first, because a layer may only be
 * built once every value it points at has found its final place.
 */
class graph_reader
{
public:
    nx_network::contents read(onnx_source const& source);

private:
    /// A bare value carrying whatever the encodings file says about @p name, for the values the
    /// graph mentions but never describes.
    value_description described_by_encoding(std::string const& name) const;

    /// The position of @p described in the value table, adding it when it is new and filling in
    /// what an earlier mention of the same name did not know.
    std::size_t remember(value_description&& described);

    void collect_values(::onnx::GraphProto const& graph);
    void collect_layers(::onnx::ModelProto const& model);

    std::vector<value_description> resolve(name_list const& names) const;
    std::vector<encoded::shared>   weights_of(name_list const& names) const;

    nx_network::contents   m_parts{};
    decode::external_files m_externals{};
    quantization_map       m_encodings{};
};

nx_network::contents
graph_reader::read(onnx_source const& source)
{
    auto const model_bytes = map_whole(source.model);

    if (model_bytes->size_bytes() > std::numeric_limits<int>::max())
    {
        throw std::runtime_error("[nx] '" + source.model.generic_string() +
                                 "' is larger than a protobuf message may be; its weights belong "
                                 "in external files");
    }

    ::onnx::ModelProto model{};
    if (not model.ParseFromArray(model_bytes->ptr(), static_cast<int>(model_bytes->size_bytes())))
    {
        throw std::runtime_error("[nx] '" + source.model.generic_string() +
                                 "' is not a parsable onnx model");
    }

    if (not source.encodings.empty() and not fs::exists(source.encodings))
    {
        throw std::runtime_error("[nx] no encodings at '" + source.encodings.generic_string() +
                                 "'; leave the path empty for a model that is not quantized");
    }

    m_encodings = read_aimet_encodings(source.encodings);

    for (auto const& path : source.external_data)
    {
        // external_data names weight files the way the model spells them, which is a file name
        m_externals.insert_or_assign(path.filename().generic_string(), map_whole(path));
    }

    collect_values(model.graph());
    collect_layers(model);

    return std::move(m_parts);
}

value_description
graph_reader::described_by_encoding(std::string const& name) const
{
    auto const found = m_encodings.find(name);

    return value_description{
        .name  = name,
        .quant = found == m_encodings.end() ? std::nullopt : std::optional{found->second},
    };
}

std::size_t
graph_reader::remember(value_description&& described)
{
    auto const [entry, is_new] = m_parts.by_name.try_emplace(described.name, m_parts.values.size());

    if (is_new)
    {
        m_parts.values.emplace_back(std::move(described));
        return entry->second;
    }

    // The same value is described from several places -- value_info, the input list, the
    // initializer that an old opset also lists as an input -- and each of them may know something
    // the others left out. Whoever spoke first keeps what it said.
    auto& kept = m_parts.values[entry->second];

    if (not kept.dtype)
    {
        kept.dtype = described.dtype;
    }
    if (not kept.layout)
    {
        kept.layout = std::move(described.layout);
    }
    if (not kept.quant)
    {
        kept.quant = std::move(described.quant);
    }

    return entry->second;
}

void
graph_reader::collect_values(::onnx::GraphProto const& graph)
{
    // value_info first: where it exists it is the most complete description there is
    for (auto& described : decode::values(graph.value_info(), m_encodings))
    {
        remember(std::move(described));
    }

    for (auto& described : decode::values(graph.input(), m_encodings))
    {
        m_parts.inputs.emplace_back(remember(std::move(described)));
    }

    for (auto& described : decode::values(graph.output(), m_encodings))
    {
        m_parts.outputs.emplace_back(remember(std::move(described)));
    }

    m_parts.weights = decode::initializers(graph.initializer(), m_externals, m_encodings);

    for (auto const& [name, weight] : m_parts.weights)
    {
        remember(value_description{
            .name   = name,
            .dtype  = weight->dtype(),
            .layout = weight->layout(),
            .quant  = weight->fmt().quant,
        });
    }

    // What is left are the edges between layers. Their shape is unknown until inference runs, but
    // they still have to exist for a layer to be able to point at them -- and an encodings file
    // names them whether or not the graph bothered to. A decode-shaped export ships with no
    // value_info at all, so this is where nearly every activation gets its quantization; taking
    // it only from the value_info list would silently leave that export unquantized.
    for (auto const& node : graph.node())
    {
        for (auto const& name : node.input())
        {
            if (not name.empty())
            {
                remember(described_by_encoding(name));
            }
        }

        for (auto const& name : node.output())
        {
            if (not name.empty())
            {
                remember(described_by_encoding(name));
            }
        }
    }
}

std::vector<value_description>
graph_reader::resolve(name_list const& names) const
{
    std::vector<value_description> resolved{};
    resolved.reserve(static_cast<std::size_t>(names.size()));

    for (auto const& name : names)
    {
        if (name.empty())
        {
            // an omitted optional operand still occupies its position
            resolved.emplace_back(nullptr);
            continue;
        }

        auto const found = m_parts.by_name.find(name);
        if (found == m_parts.by_name.end())
        {
            throw std::logic_error("[nx] a node reads '" + name +
                                   "', which the graph never mentions");
        }

        resolved.emplace_back(m_parts.values[found->second]);
    }

    return resolved;
}

std::vector<encoded::shared>
graph_reader::weights_of(name_list const& names) const
{
    std::vector<encoded::shared> parameters{};

    for (auto const& name : names)
    {
        auto const found = m_parts.weights.find(name);
        if (found != m_parts.weights.end())
        {
            parameters.emplace_back(found->second);
        }
    }

    return parameters;
}

void
graph_reader::collect_layers(::onnx::ModelProto const& model)
{
    std::unordered_map<std::string, std::int64_t> opset{}; // domain -> version
    for (auto const& imported : model.opset_import())
    {
        opset.insert_or_assign(canonical_domain(imported.domain()), imported.version());
    }

    auto const& registry = op_schema_registry();

    std::vector<layer_description::shared> layers{};
    layers.reserve(static_cast<std::size_t>(model.graph().node_size()));

    for (auto const& node : model.graph().node())
    {
        auto const domain = canonical_domain(node.domain());

        std::int64_t version = 0;
        if (auto const imported = opset.find(domain); imported != opset.end())
        {
            version = imported->second;
        }
        else
        {
            spdlog::warn("[nx] the model imports no operator set for domain '{}', so '{}' is "
                         "left without a signature",
                         domain,
                         node.op_type());
        }

        // the name is optional in onnx; the first result names the node just as well and is
        // unique by construction
        auto name = node.name();
        if (name.empty())
        {
            name = node.output_size() > 0 ? node.output(0)
                                          : node.op_type() + "_" + std::to_string(layers.size());
        }

        nx_layer::descriptor described{
            .name       = std::move(name),
            .op_type    = node.op_type(),
            .domain     = domain,
            .opset      = version,
            .schema     = registry.find(node.op_type(), domain, version),
            .operands   = resolve(node.input()),
            .results    = resolve(node.output()),
            .attributes = decode::attributes(node.attribute()),
            .parameters = weights_of(node.input()),
        };

        layers.emplace_back(std::make_shared<nx_layer>(std::move(described)));
    }

    m_parts.layers = in_dependency_order(layers);
}

} // namespace

nx_network::nx_network(contents&& parts) : m_contents(std::move(parts))
{
}

nx_network::~nx_network() = default;

std::size_t
nx_network::n_inputs() const
{
    return m_contents.inputs.size();
}

std::size_t
nx_network::n_outputs() const
{
    return m_contents.outputs.size();
}

value_description const*
nx_network::input_description(std::size_t index) const
{
    return index < m_contents.inputs.size() ? &m_contents.values[m_contents.inputs[index]]
                                            : nullptr;
}

value_description const*
nx_network::output_description(std::size_t index) const
{
    return index < m_contents.outputs.size() ? &m_contents.values[m_contents.outputs[index]]
                                             : nullptr;
}

value_description const*
nx_network::description(std::size_t index) const
{
    return index < m_contents.values.size() ? &m_contents.values[index] : nullptr;
}

value_description const*
nx_network::description(std::string_view name) const
{
    auto const found = m_contents.by_name.find(std::string{name});

    return found != m_contents.by_name.end() ? &m_contents.values[found->second] : nullptr;
}

encoded::shared
nx_network::parameter(std::string_view name) const
{
    auto const found = m_contents.weights.find(std::string{name});

    return found != m_contents.weights.end() ? found->second : nullptr;
}

std::span<layer_description::shared const>
nx_network::layers() const
{
    return m_contents.layers;
}

network_description::shared
load(onnx_source const& source)
{
    return std::make_shared<nx_network>(graph_reader{}.read(source));
}

} // namespace lnpu::nex::nx
