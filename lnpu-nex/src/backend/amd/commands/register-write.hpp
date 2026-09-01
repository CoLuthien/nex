
#pragma once

#include "amd/command.hpp"

namespace lnpu::nex::amd::commands
{

/**
 * @brief Writes one word to a register of the tile named by the placement.
 *
 * The other writers here work out which register they mean from the placement: queue_push knows
 * the task queues, issue_token knows the queue's token field. This one is told, which is what a
 * caller needs when the register is not part of any of those blocks.
 *
 * Two uses so far, both in designs whose cores loop a number of times the instruction stream
 * decides:
 *
 *   - runtime parameters. A core reads its trip counts out of its own data memory, and this is
 *     what puts them there. Where in data memory is up to whoever compiled the design, so the
 *     address travels with the design rather than being worked out here.
 *   - releasing the cores, once everything they read from is configured.
 *
 * Only the tile coordinate of the placement is read. A register is not reached through a buffer
 * descriptor or a channel, so #bd, #channel and #direction say nothing here.
 */
class register_write : public amd::command
{
public:
    struct parameters
    {
        word address{}; ///< Register offset within the tile.
        word value{};
    };

    explicit register_write(parameters param);

    void emit(npu::placement const& where, std::vector<word>& out) const override;

private:
    parameters m_param;
};

} // namespace lnpu::nex::amd::commands
