
#pragma once

#include "nex/frontend/network-description.hpp"

#include <string>
#include <vector>

namespace sample
{

/**
 * @brief The outcome of one check over the whole graph.
 *
 * A check walks everything it applies to, counts what it looked at, and records what it did not
 * like. Findings are kept whole rather than reduced to a count: the first few are what say
 * whether a failure is one broken tensor or a misread file.
 */
struct verdict
{
    std::string what{};
    std::size_t examined{};

    /// One line about what the check saw, printed whether or not it found anything.
    std::string summary{};

    std::vector<std::string> findings{};

    /**
     * @brief Whether a finding is a fault of the parse or only something to know about it.
     *
     * An advisory check reports on the model, not on the loader -- how much of the graph has a
     * signature, how much of it is quantized -- and never decides the exit code, because a model
     * is allowed to hold operators this frontend has no schema for.
     */
    bool advisory = false;

    void note(std::string finding) { findings.emplace_back(std::move(finding)); }
    bool passed() const { return findings.empty(); }
};

/// Everything the checks were told about how far to look; see options::sampled.
struct depth
{
    std::size_t sampled = 4096;
};

using network = lnpu::nex::network_description;

/// What the graph holds, printed before anything is judged.
void summarise(network const& graph);

/// The first @p count layers, with their operands, results and weights.
void list_layers(network const& graph, std::size_t count);

/*
 * The checks. Each is independent of the others and reads only the public frontend interface --
 * which is the point: this is the surface a backend is written against, so a gap here is a gap
 * every consumer of the loader would meet.
 */

/// Graph inputs and outputs exist, are named, and are the same values the table hands out.
verdict check_interface(network const& graph);

/// The value table agrees with itself by index and by name.
verdict check_value_table(network const& graph);

/// Every operand and result of every layer resolves to a value the graph knows.
verdict check_operands(network const& graph);

/// Nothing is read before it is written, and no two layers write the same value.
verdict check_dependency_order(network const& graph);

/// A layer's operand and result counts fit the signature it was resolved to (advisory).
verdict check_signatures(network const& graph);

/// Weights are reachable both ways, sized as their shape says, and their bytes are readable.
verdict check_parameters(network const& graph, depth how_far);

/// Quantization parameters are complete and well formed wherever they are present.
verdict check_quantization(network const& graph);

/// The stored weights fall inside the code range their encodings describe.
verdict check_encodings_fit_weights(network const& graph, depth how_far);

/**
 * @brief Prints @p results and answers whether the parse is to be trusted.
 *
 * @return true when no check that decides the exit code found anything.
 */
bool conclude(std::vector<verdict> const& results);

} // namespace sample
