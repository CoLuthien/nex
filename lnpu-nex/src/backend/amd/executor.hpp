

#pragma once

#include "layer.hpp"
#include "device.hpp"

#include "nex/backend/network.hpp"
#include "nex/frontend/network-description.hpp"

#include <cstddef>
#include <map>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

namespace lnpu::nex::amd
{

/**
 * @brief One graph, run layer by layer, fed and read at its boundary.
 *
 * The layers are built from the description once and each is driven by the names its own program
 * lowered. What this adds on top is the routing: which layers read a value the caller supplies,
 * and which layer writes one the caller wants back. Both tables are built while the layers are,
 * because that is where the graph is still being read.
 *
 * Only the boundary is routed. Everything else is settled during construction and never asked of a
 * caller: weights belong to the network and do not change, and a value produced inside the graph
 * and consumed inside it is given a window of one arena this plans for itself. Planning it here is
 * deliberate but temporary -- what to do with the room a finished value leaves behind is a decision
 * a caller may want to make, and this holds it only until there is somewhere better for it to live.
 */
class executor : public nex::executor
{
    amd::device*               m_dev;
    std::vector<layer::unique> m_layers;

    /// Declaration order, which is the order io_contract counts in. Copied rather than pointed at,
    /// so the description need not outlive the executor prepared from it.
    std::vector<encoded::format> m_inputs;
    std::vector<encoded::format> m_outputs;

    /// Name to index into the two above. Heterogeneous compare throughout, so a string_view key
    /// costs no string.
    std::map<std::string, std::size_t, std::less<>> m_input_index;
    std::map<std::string, std::size_t, std::less<>> m_output_index;

    /// Where a boundary value goes. An input may feed several layers, so that one fans out; an
    /// output has a single producer, so it does not. These are pointers to the layers themselves
    /// rather than into #m_layers, which is why collecting them while that vector still grows is
    /// safe.
    std::map<std::string, std::vector<layer*>, std::less<>> m_entry;
    std::map<std::string, layer*, std::less<>>              m_exit;

    /// One allocation behind every value the graph both produces and consumes. Held rather than
    /// left to the windows cut from it: the windows would keep it alive anyway, but the arena is
    /// the graph's own storage and reads better as such.
    buffer::shared m_arena;

    /// The interior values, each a window into #m_arena, bound to their producer and consumers at
    /// construction. Kept because the driver resolves addresses through what these name.
    std::vector<encoded::shared> m_interior;

public:
    /**
     * @throws std::runtime_error when the graph and the backend do not line up: an operation that
     *         is not registered, a layer that would not lower, a weight a layer will not take, a
     *         graph input no layer reads, or a graph output no layer offers as a result. Every one
     *         of them would otherwise surface much later as an execute() that quietly did nothing
     *         useful.
     */
    explicit executor(network_description const& description, amd::device* dev);

    std::error_code execute() override;

    std::error_code set_input(std::string_view key, encoded::shared target) override;
    std::error_code set_output(std::string_view key, encoded::shared target) override;

    std::error_code reset_input(std::string_view key) override;
    std::error_code reset_output(std::string_view key) override;

    std::size_t input_count() const override;
    std::size_t output_count() const override;

    encoded::format const* input_description(int index) const override;
    encoded::format const* input_description(std::string_view name) const override;

    encoded::format const* output_description(int index) const override;
    encoded::format const* output_description(std::string_view name) const override;

    bool has_input(std::string_view name) const override;
    bool has_output(std::string_view name) const override;

private:
    /// Reads the graph's own inputs and outputs into the tables io_contract answers from.
    void collect_boundary(network_description const& description);

    /// Files @p ready under every boundary value it touches, and binds the weights it reads.
    void route(layer_description const&   node,
               network_description const& description,
               layer*                     ready);

    /// Refuses a graph whose boundary is not reachable from any layer.
    void verify_boundary() const;

    /// Sizes every value interior to the graph, packs them into one arena, and binds each layer to
    /// its window of it.
    void plan_interior(network_description const& description);
};

} // namespace lnpu::nex::amd
