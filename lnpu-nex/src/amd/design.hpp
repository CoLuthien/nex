#pragma once

#include "command.hpp"

#include <cstdint>
#include <vector>

namespace lnpu::nex::amd
{

/**
 * @brief What every xclbin fixed, whatever it computes.
 *
 * These are compiled into the core programs when the xclbin is baked, so nothing at runtime may
 * choose them -- the instruction stream only decides how many times that fixed arrangement is
 * walked. A value here that disagrees with the xclbin it came from fails nothing and merely
 * produces wrong results, so the baking script writes it into the xclbin's USER_METADATA section
 * and descriptor::common() reads it back rather than it being restated in C++.
 *
 * Only what is common lives here. What one operator fixes and another has no word for -- a gemm's
 * tile, whether an rmsnorm was baked with a weight stream -- belongs to that operator's own design
 * type, which its program declares and its describe() fills in. That keeps the code that knows
 * what a field means in one place, instead of spreading it over a loader that would have to know
 * every operator to hold their fields.
 *
 * How the cores are laid out is one of those. Only a design that addresses a core by tile needs
 * to know, and only such a design can say truthfully: an rmsnorm reaches its cores through fifos
 * and never names one, so whatever row count it stated would be a number nothing could check --
 * and its weighted variant runs two cores per channel anyway, on rows its placer chose.
 */
struct design
{
    npu::generation generation{};

    /// Goes in header word 1. This is the partition width, not the number of columns the design
    /// puts to work -- a one-column design still reports the full partition (see docs/08 8.4).
    std::uint32_t partition_columns{};

    /// Columns the design actually spreads work over.
    std::uint32_t columns{};

    /**
     * @brief Where in a core's data memory its runtime parameters live.
     *
     * A design whose cores loop a number of times the instruction stream decides reads those
     * counts from here. The compiler puts the buffer wherever it had room -- the same operator
     * compiled with a different tile size lands at a different address -- so this cannot be
     * worked out from the operation and has to come with the design.
     *
     * Empty for a design whose cores need telling nothing.
     */
    std::vector<std::uint32_t> parameter_slots;

    /// Register that releases a core once what it reads from is configured. Zero when the design
    /// has no cores to release, which is the same designs that carry no parameter slots.
    std::uint32_t start_register{};
};

/**
 * @brief Where a transfer's base address comes from: a kernel argument and a byte offset into it.
 *
 * The address is not known when the commands are written. ddr_patch leaves a request for XRT to
 * fill it in from the bound buffer just before the run, which is what lets one instruction stream
 * serve a buffer that moves.
 *
 * Take the index from descriptor::argument() rather than counting the arguments by hand. An index
 * off by one reads a different buffer, which is a thing the hardware does without complaint.
 */
struct binding
{
    command::word argument_index{};
    command::word offset_bytes{};
};

} // namespace lnpu::nex::amd
