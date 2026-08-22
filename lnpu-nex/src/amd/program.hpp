#pragma once

#include "command.hpp"
#include "design.hpp"

#include <cstdint>
#include <memory>
#include <system_error>

namespace lnpu::nex::amd
{

/**
 * @brief One operation, lowered onto the one xclbin design that was baked for it.
 *
 * Everything about what to compute and where it lives is settled by the constructor; wire() only
 * unfolds that into commands. The split earns its keep on the decode path, which does not rebuild
 * its sequence per token -- it wires once, bakes an ELF, and lets ddr_patch carry what moves.
 *
 * A program neither executes nor owns XRT resources; that is operation's job. Programs are made
 * through operation::make(), which is what pairs a program with the design it was baked for and
 * the only place the two can be mismatched -- after that the pairing is a fact. One design serves
 * as many programs as the model has calls to it: a single gemm xclbin runs every projection.
 *
 * Holding the design as a value rather than reaching through an operation is deliberate: it is
 * what lets a program be built and checked on a machine with no NPU in it.
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
