#pragma once

#include "program.hpp"

#include "nex/nex-types.hpp"

#include <cstdint>
#include <optional>
#include <string_view>
#include <system_error>

namespace lnpu::nex
{
class layer_description;
}

namespace lnpu::nex::amd
{
class descriptor;

/**
 * @brief What it takes to turn one layer into one program, with the operator's type erased.
 *
 * Every program declares its own parameters type and its own design, so "which program runs this
 * layer" is a choice of C++ type rather than of value -- which is what would otherwise make this
 * an if/else over op_type at the one place a type has to be named. A builder is that choice,
 * already made: the table holds one per operator and the caller only calls it.
 *
 * Where the buffers are is not in here. A binding names the argument slot the design expects a
 * tensor in, which the descriptor knows, and nothing more: which buffer is bound to that slot is
 * settled outside by set_input()/set_output(), so no part of this has to know how a network laid
 * its tensors out.
 */
using program_builder = program::shared (*)(descriptor const&,
                                            layer_description const&,
                                            std::error_code&);

struct lowering
{
    /// The descriptor "op" of the design this layer needs, so the wrong xclbin can be caught
    /// before a program is built against it. Taken from the program's own kOp, never restated.
    std::string_view design_op;

    program_builder build;
};

/**
 * @brief How to run an operator, or nullptr when this build cannot run it at all.
 *
 * One hash lookup. An operator this build has no program for is a normal answer -- a graph may
 * name anything onnx defines -- so it is reported by returning nothing rather than by throwing.
 */
lowering const* find_lowering(std::string_view op_type);

/// @name Reading a shape off a layer
/// The same job whatever the operator, so every lower() shares it rather than growing its own.
/// @{

/// What the graph said a value's extents are, or nullptr when it never said.
inline lnpu::layout const*
shape_of(value_description const* value)
{
    if (nullptr == value or not value->layout.has_value()) return nullptr;

    return &value->layout.value();
}

/**
 * @brief Elements the extents from @p from onwards hold.
 *
 * Nothing when one of them is not a fixed positive number. A dynamic dimension is not a shape
 * anything here can emit descriptors for, and an extent of zero would emit a run of no elements
 * that the hardware would accept and say nothing about.
 */
inline std::optional<std::uint64_t>
elements_in(lnpu::layout const& shape, lnpu::layout::rank_type from = 0)
{
    std::uint64_t total = 1;

    for (auto at = from; at < shape.rank(); ++at)
    {
        auto const extent = shape.extent(at);
        if (extent <= 0) return std::nullopt;

        total *= static_cast<std::uint64_t>(extent);
    }

    return total;
}

/// @}

} // namespace lnpu::nex::amd
