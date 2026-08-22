#pragma once

#include "command.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <system_error>
#include <vector>

namespace lnpu::nex::amd
{

/**
 * @brief What the xclbin fixed, carried as data rather than as constants in here.
 *
 * These are compiled into the core programs when the xclbin is baked, so nothing at runtime may
 * choose them -- the instruction stream only decides how many times that fixed arrangement is
 * walked. A value here that disagrees with the xclbin it came from fails nothing and merely
 * produces wrong results, so the baking script writes it into the xclbin's USER_METADATA section
 * and it is read back from there rather than restated in C++.
 */
struct design
{
    /// Which program reads this design. The orchestrator pairs the two.
    std::string op;

    npu::generation generation{};

    /// Goes in header word 1. This is the partition width, not the number of columns the design
    /// puts to work -- a one-column design still reports the full partition (see docs/08 8.4).
    std::uint32_t partition_columns{};

    /// Columns the design actually spreads work over.
    std::uint32_t columns{};

    /// Core rows a column puts to work, counted from the first compute row.
    std::uint32_t rows{};

    /// The first compute row. Row 0 is the shim and row 1 the mem tile, so cores start at 2.
    std::uint32_t first_core_row{2};

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

    /// Register that releases a core once what it reads from is configured.
    std::uint32_t start_register{};

    /// The order kernel arguments appear in, which is the order ddr_patch indexes them by.
    /// Getting this wrong reads the wrong buffer and nothing complains.
    std::vector<std::string> args;
};

/**
 * @brief Where a transfer's base address comes from: a kernel argument and a byte offset into it.
 *
 * The address is not known when the commands are written. ddr_patch leaves a request for XRT to
 * fill it in from the bound buffer just before the run, which is what lets one instruction stream
 * serve a buffer that moves.
 */
struct binding
{
    command::word argument_index{};
    command::word offset_bytes{};
};

/**
 * @brief One operation, lowered onto the one xclbin design that was baked for it.
 *
 * Everything about what to compute and where it lives is settled by the constructor; wire() only
 * unfolds that into commands. The split earns its keep on the decode path, which does not rebuild
 * its sequence per token -- it wires once, bakes an ELF, and lets ddr_patch carry what moves.
 *
 * A program neither executes nor owns XRT resources; that is operation's job. One program belongs
 * to one operation, and the orchestrator is what pairs them. Holding the design as a value rather
 * than reaching through an operation is deliberate: it is what lets a program be built and checked
 * on a machine with no NPU in it.
 */
class program
{
public:
    using shared = std::shared_ptr<program>;

    virtual ~program() = default;

    program(program const&)            = delete;
    program& operator=(program const&) = delete;

    /// Appends the commands that carry out this operation to @p sequence.
    virtual std::error_code wire(command_list& sequence) const = 0;

    /// Buffer descriptors this program claims on its busiest column, so that several programs
    /// sharing one sequence can be handed ranges that do not overlap.
    virtual std::uint32_t buffer_descriptors_used() const = 0;

protected:
    program() = default;
};

} // namespace lnpu::nex::amd
