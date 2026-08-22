
#pragma once

#include "amd/command.hpp"

namespace lnpu::nex::amd::commands
{
/** AI Asissted code
 */

// XAIE_IO_WRITE against a shim DMA task queue: hands a previously configured
// buffer descriptor to the DMA engine, which is what actually starts the
// transfer. Recording a BD and starting it are separate steps, so several BDs
// can be laid down first and pushed in whatever order the schedule wants.
//
// Requires that block_write and ddr_patch for the same (tile, bd) come first.
class queue_push : public amd::command
{
public:
    struct parameters
    {
        // Number of extra times the descriptor repeats; zero runs it once.
        word repeat_count{};

        // Raise a completion token when the transfer finishes. Only a
        // transfer whose token is raised can be awaited with tct_wait, so
        // device-to-host transfers normally set it.
        bool issue_token{false};
    };

    explicit queue_push(parameters param);

    void emit(npu::placement const& where, std::vector<word>& out) const override;

private:
    parameters m_param;
};

} // namespace lnpu::nex::amd::commands
