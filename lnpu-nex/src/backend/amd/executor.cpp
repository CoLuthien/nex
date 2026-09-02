
#include "executor.hpp"
#include "operation.hpp"
#include "nex-utils.hpp"

#include "cmn/buffer/mapped-buffer.hpp"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <map>
#include <numeric>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace lnpu::nex::amd
{

namespace
{

std::error_code
refuse(std::string_view tensor, std::string const& why)
{
    spdlog::error("[nex::amd::executor] '{}' {}", tensor, why);
    return std::make_error_code(std::errc::invalid_argument);
}

/**
 * @brief What each value in the arena starts on.
 *
 * XRT registers caller storage by pinning the pages behind it, so two values sharing a page are two
 * kernel arguments the driver has to track over the same pinned page. Starting each on its own page
 * keeps that from ever arising. The padding it leaves between small values costs address space and
 * not memory: an anonymous mapping commits a page when it is first touched, and nothing ever
 * touches the gaps.
 */
constexpr std::size_t argument_alignment = 4096;

std::size_t
align_up(std::size_t bytes)
{
    return (bytes + argument_alignment - 1) / argument_alignment * argument_alignment;
}

/**
 * @brief One value the graph both writes and reads, and the stretch of the run it has to survive.
 */
struct interior
{
    encoded::format     what;
    layer*              producer{};
    std::vector<layer*> consumers;

    /// Indices into the layer sequence, which is in dependency order and so is also the order it
    /// runs in. The value is live from the layer that writes it through the last one that reads it.
    std::size_t first{};
    std::size_t last{};

    std::size_t size{};
    std::size_t offset{};
};

bool
overlap(interior const& lhs, interior const& rhs)
{
    // Touching at a single layer still counts: that is the layer reading one of them while it
    // writes the other, and both sets of bytes are being addressed at the same moment.
    return lhs.first <= rhs.last and rhs.first <= lhs.last;
}

/**
 * @brief What the graph states about an interior value, refusing anything it left out.
 *
 * A boundary value may be described loosely, because a caller brings its own buffer and the layer
 * checks it against whatever the graph did say. This is the opposite case: nobody brings a buffer
 * for these, so a type or a shape the graph never stated is a byte count that cannot be worked out,
 * and the graph is turned away rather than guessed at.
 */
encoded::format
must_state(value_description const& value, std::string_view whose)
{
    if (not value.dtype)
    {
        throw std::runtime_error("[nex::amd::executor] '" + value.name + "', which '" +
                                 std::string{whose} +
                                 "' produces for the graph's own use, has no type in the "
                                 "description, so there is no telling how much room it needs");
    }

    if (not value.layout)
    {
        throw std::runtime_error("[nex::amd::executor] '" + value.name + "', which '" +
                                 std::string{whose} +
                                 "' produces for the graph's own use, has no shape in the "
                                 "description, so there is no telling how much room it needs");
    }

    return encoded::format{
        .name     = value.name,
        .encoding = *value.dtype,
        .layout   = *value.layout,
        .quant    = value.quant,
    };
}

std::size_t
room_for(encoded::format const& what)
{
    if (not has_fixed_element_size(what.encoding))
    {
        throw std::runtime_error("[nex::amd::executor] '" + what.name +
                                 "' is stored in an encoding with no fixed element size, which is "
                                 "not addressable as an array and so cannot be given a region");
    }

    if (what.layout.is_dynamic())
    {
        throw std::runtime_error("[nex::amd::executor] '" + what.name +
                                 "' still has an unresolved extent (" + what.layout.to_string() +
                                 "), and a shape that is not settled has no size to allocate");
    }

    auto const bytes = what.layout.element_count() * in_bytes(what.encoding);
    if (0 == bytes)
    {
        throw std::runtime_error("[nex::amd::executor] '" + what.name +
                                 "' works out to no bytes at all (" + what.layout.to_string() +
                                 "), and an empty region is not something a kernel can be given");
    }

    return bytes;
}

/**
 * @brief Gives every value an offset in one arena and returns how big that arena has to be.
 *
 * Largest first, then lowest fit: each value goes at the lowest offset that clears everything alive
 * at the same time as it, leaving the smaller ones to fill the gaps rather than the other way
 * round. Packing these is NP-hard, so this is a heuristic and not a minimum -- but it is the one
 * that reuses the room a finished value leaves behind, which is the whole point of an arena over an
 * allocation each.
 *
 * The order is settled by size and then by first use, never by name or by discovery, so the same
 * graph is packed the same way every time.
 */
std::size_t
place(std::vector<interior>& values)
{
    std::vector<std::size_t> order(values.size());
    std::iota(order.begin(), order.end(), 0U);
    std::ranges::sort(order, [&values](std::size_t lhs, std::size_t rhs) {
        if (values[lhs].size != values[rhs].size) return values[lhs].size > values[rhs].size;
        return values[lhs].first < values[rhs].first;
    });

    std::vector<std::size_t> standing{};
    std::size_t              total = 0;

    for (auto const at : order)
    {
        auto& value = values[at];

        // Everything already placed that is alive while this one is, as [begin, end) byte ranges
        // sorted low to high -- which is what lets the search below stop at the first gap.
        std::vector<std::pair<std::size_t, std::size_t>> occupied{};
        for (auto const other : standing)
        {
            if (not overlap(value, values[other])) continue;
            occupied.emplace_back(values[other].offset, values[other].offset + values[other].size);
        }
        std::ranges::sort(occupied);

        std::size_t candidate = 0;
        for (auto const& block : occupied)
        {
            if (candidate + value.size <= block.first) break;
            candidate = std::max(candidate, align_up(block.second));
        }

        value.offset = candidate;
        total        = std::max(total, candidate + value.size);
        standing.emplace_back(at);
    }

    return total;
}

} // namespace

executor::executor(network_description const& description, amd::device* dev) //
    : m_dev(dev)
{
    collect_boundary(description);

    // 'node' rather than 'layer': a loop variable of that name hides the class of the same name,
    // and layer::create() below would then not name a type at all.
    for (auto& node : description.layers())
    {
        std::error_code ec{};

        auto* op = m_dev->op(node->op_type());
        if (not op)
        {
            throw std::runtime_error("[nex::amd::executor] no operation is registered for '" +
                                     std::string{node->op_type()} + "', which '" +
                                     std::string{node->name()} + "' is");
        }

        auto executable = op->create_instance(node, ec);

        if (ec)
        {
            // create_instance() has already said which of its three refusals this was.
            throw std::runtime_error("[nex::amd::executor] '" + std::string{node->name()} +
                                     "' would not lower: " + ec.message());
        }

        // create() logs why before it hands back nothing, so there is nothing to add here -- but
        // an unchecked nullptr would sit in the vector until execute() dereferenced it.
        auto ready = layer::create(*node, std::move(executable));
        if (nullptr == ready)
        {
            throw std::runtime_error("[nex::amd::executor] '" + std::string{node->name()} +
                                     "' and the program lowered for it do not agree");
        }

        m_layers.emplace_back(std::move(ready));
        route(*node, description, m_layers.back().get());
    }

    verify_boundary();
    plan_interior(description);
}

void
executor::collect_boundary(network_description const& description)
{
    for (std::size_t i = 0; i < description.n_inputs(); ++i)
    {
        auto const* value = description.input_description(i);
        if (nullptr == value) continue;

        // Older opsets list a graph's initializers among its inputs as well. Those are weights the
        // network carries, not anything a caller is asked to supply, so this is what keeps them
        // off the boundary -- without it every weight would be reported as an input.
        if (description.parameter(value->name)) continue;

        // A name the graph states twice is one value, not two.
        if (not m_input_index.emplace(value->name, m_inputs.size()).second) continue;

        m_inputs.emplace_back(stated_as(*value));
    }

    for (std::size_t i = 0; i < description.n_outputs(); ++i)
    {
        auto const* value = description.output_description(i);
        if (nullptr == value) continue;

        if (not m_output_index.emplace(value->name, m_outputs.size()).second) continue;

        m_outputs.emplace_back(stated_as(*value));
    }
}

void
executor::route(layer_description const& node, network_description const& description, layer* ready)
{
    for (std::size_t i = 0; i < node.input_count(); ++i)
    {
        auto const name = node.input_name(i);

        // An optional operand the node left out still occupies its position, and has no name.
        if (name.empty()) continue;

        // A tensor the program resolved while lowering is not an argument, so there is nothing to
        // route to it. Asking first keeps it out of the tables rather than discovering it as a
        // refusal on every bind.
        if (not ready->takes(name)) continue;

        if (auto weight = description.parameter(name))
        {
            // A weight is the network's own and does not change between runs, so it is bound here
            // once instead of being asked of the caller every time.
            if (auto const ec = ready->set_input(name, std::move(weight)))
            {
                throw std::runtime_error("[nex::amd::executor] '" + std::string{node.name()} +
                                         "' would not take the weight '" + std::string{name} +
                                         "': " + ec.message());
            }

            continue;
        }

        if (m_input_index.contains(name))
        {
            m_entry[std::string{name}].emplace_back(ready);
        }
    }

    for (std::size_t i = 0; i < node.output_count(); ++i)
    {
        auto const name = node.output_name(i);

        if (name.empty()) continue;
        if (not ready->takes(name)) continue;
        if (not m_output_index.contains(name)) continue;

        if (not m_exit.emplace(name, ready).second)
        {
            throw std::runtime_error("[nex::amd::executor] '" + std::string{name} +
                                     "' is written by more than one layer, so there is no single "
                                     "place to send what a caller asks for it");
        }
    }
}

void
executor::verify_boundary() const
{
    for (auto const& stated : m_input_index)
    {
        if (not m_entry.contains(stated.first))
        {
            throw std::runtime_error("[nex::amd::executor] '" + stated.first +
                                     "' is an input of this graph that no layer takes as an "
                                     "argument, so nothing set on it would ever be read");
        }
    }

    for (auto const& stated : m_output_index)
    {
        if (not m_exit.contains(stated.first))
        {
            throw std::runtime_error("[nex::amd::executor] '" + stated.first +
                                     "' is an output of this graph that no layer offers as a "
                                     "result, so nothing would ever be written to it");
        }
    }
}

void
executor::plan_interior(network_description const& description)
{
    auto const nodes = description.layers();

    std::vector<interior>                           values{};
    std::map<std::string, std::size_t, std::less<>> found{};

    // m_layers was filled from this same sequence, one for one, so an index means the same layer in
    // both -- and because the sequence is in dependency order, it is also when that layer runs.
    for (std::size_t at = 0; at < nodes.size(); ++at)
    {
        auto const& node  = *nodes[at];
        auto*       ready = m_layers[at].get();

        // Operands before results: a value this layer reads was written by an earlier one, and
        // looking them up first is what keeps a node from ever finding its own result.
        for (std::size_t i = 0; i < node.input_count(); ++i)
        {
            auto const name = node.input_name(i);

            if (name.empty()) continue;
            if (not ready->takes(name)) continue;
            if (m_input_index.contains(name) or m_output_index.contains(name)) continue;
            if (description.parameter(name)) continue;

            auto const seen = found.find(name);
            if (seen == found.end())
            {
                throw std::runtime_error("[nex::amd::executor] '" + std::string{node.name()} +
                                         "' reads '" + std::string{name} +
                                         "', which is not an input of the graph, not a weight, and "
                                         "not something an earlier layer offers as a result");
            }

            values[seen->second].last = at;
            values[seen->second].consumers.emplace_back(ready);
        }

        for (std::size_t i = 0; i < node.output_count(); ++i)
        {
            auto const name = node.output_name(i);

            if (name.empty()) continue;
            if (not ready->takes(name)) continue;
            if (m_input_index.contains(name) or m_output_index.contains(name)) continue;
            if (description.parameter(name)) continue;

            auto const* stated = node.output(name);
            if (nullptr == stated)
            {
                throw std::runtime_error("[nex::amd::executor] '" + std::string{node.name()} +
                                         "' writes '" + std::string{name} +
                                         "', which its own description does not describe");
            }

            if (not found.emplace(name, values.size()).second)
            {
                throw std::runtime_error("[nex::amd::executor] '" + std::string{name} +
                                         "' is written by more than one layer, so there is no one "
                                         "region it stands for");
            }

            auto what = must_state(*stated, node.name());
            auto size = room_for(what);

            values.emplace_back(interior{
                .what     = std::move(what),
                .producer = ready,
                .first    = at,
                .last     = at,
                .size     = size,
            });
        }
    }

    if (values.empty()) return;

    auto const total = place(values);

    // One region of bytes, addressed as bytes: what the arena is measured in is byte offsets, and
    // the layouts that matter are the ones cut out of it.
    std::error_code ec{};
    m_arena = allocate_mapped(lnpu::layout{{static_cast<lnpu::layout::extent_type>(total)}}, //
                              EDataType::u8,
                              ec);
    if (ec or nullptr == m_arena)
    {
        throw std::runtime_error("[nex::amd::executor] the graph needs " + std::to_string(total) +
                                 " bytes for the values it keeps to itself, which could not be "
                                 "allocated: " + ec.message());
    }

    m_interior.reserve(values.size());
    for (auto& value : values)
    {
        auto const name = value.what.name;

        auto region =
            encoded::create(std::move(value.what), m_arena->make_view(value.offset, value.size));

        // Both directions from here on are the layer's own checks -- that the window is the type
        // and shape it was told to expect, and that XRT will take it. A refusal is the plan and the
        // graph disagreeing, which is nothing a caller did.
        if (auto const refused = value.producer->set_output(name, region))
        {
            throw std::runtime_error("[nex::amd::executor] the region planned for '" + name +
                                     "' was not accepted by the layer that writes it: " +
                                     refused.message());
        }

        for (auto* consumer : value.consumers)
        {
            if (auto const refused = consumer->set_input(name, region))
            {
                throw std::runtime_error("[nex::amd::executor] the region planned for '" + name +
                                         "' was not accepted by a layer that reads it: " +
                                         refused.message());
            }
        }

        m_interior.emplace_back(std::move(region));
    }

    spdlog::debug("[nex::amd::executor] {} interior values packed into {} bytes",
                  m_interior.size(),
                  total);
}

std::error_code
executor::execute()
{
    for (auto& one : m_layers)
    {
        if (auto const ec = one->execute())
        {
            return ec;
        }
    }

    return {};
}

std::error_code
executor::set_input(std::string_view key, encoded::shared target)
{
    auto const found = m_entry.find(key);
    if (found == m_entry.end())
    {
        return refuse(key, "is not an input of this network");
    }

    auto& consumers = found->second;

    std::error_code refused{};
    auto const      failed = std::ranges::find_if(consumers, [&](layer* consumer) { //
        refused = consumer->set_input(key, target);
        return static_cast<bool>(refused);
    });

    if (failed == consumers.end()) return {};

    // A value several layers read is bound in all of them or in none. Left bound in some, it would
    // still count towards those layers' own tally of bound arguments, and execute() would run part
    // of the graph on a buffer this call told the caller it had refused. Only the layers ahead of
    // the refusal took it, so only those are undone.
    std::for_each(consumers.begin(), failed, [&](layer* consumer) {
        std::ignore = consumer->reset_input(key);
    });

    return refused;
}

std::error_code
executor::set_output(std::string_view key, encoded::shared target)
{
    auto const found = m_exit.find(key);
    if (found == m_exit.end())
    {
        return refuse(key, "is not an output of this network");
    }

    return found->second->set_output(key, std::move(target));
}

std::error_code
executor::reset_input(std::string_view key)
{
    auto const found = m_entry.find(key);
    if (found == m_entry.end())
    {
        return refuse(key, "is not an input of this network");
    }

    // Every consumer is cleared even after one refuses: stopping halfway would leave exactly the
    // half-bound state set_input() goes out of its way to avoid. The first refusal is the one the
    // caller hears about.
    std::error_code first{};
    std::ranges::for_each(found->second, [&](layer* consumer) {
        auto const ec = consumer->reset_input(key);
        if (ec and not first) first = ec;
    });

    return first;
}

std::error_code
executor::reset_output(std::string_view key)
{
    auto const found = m_exit.find(key);
    if (found == m_exit.end())
    {
        return refuse(key, "is not an output of this network");
    }

    return found->second->reset_output(key);
}

std::size_t
executor::input_count() const
{
    return m_inputs.size();
}

std::size_t
executor::output_count() const
{
    return m_outputs.size();
}

encoded::format const*
executor::input_description(int index) const
{
    if (index < 0) return nullptr;

    auto const at = static_cast<std::size_t>(index);
    return at < m_inputs.size() ? &m_inputs[at] : nullptr;
}

encoded::format const*
executor::input_description(std::string_view name) const
{
    auto const found = m_input_index.find(name);
    return found != m_input_index.end() ? &m_inputs[found->second] : nullptr;
}

encoded::format const*
executor::output_description(int index) const
{
    if (index < 0) return nullptr;

    auto const at = static_cast<std::size_t>(index);
    return at < m_outputs.size() ? &m_outputs[at] : nullptr;
}

encoded::format const*
executor::output_description(std::string_view name) const
{
    auto const found = m_output_index.find(name);
    return found != m_output_index.end() ? &m_outputs[found->second] : nullptr;
}

bool
executor::has_input(std::string_view name) const
{
    return m_input_index.contains(name);
}

bool
executor::has_output(std::string_view name) const
{
    return m_output_index.contains(name);
}

} // namespace lnpu::nex::amd
