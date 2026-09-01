
#pragma once

#include "amd/command.hpp"

/** AI Asissted code
 */

namespace lnpu::nex::amd::commands
{

// XAIE_IO_CUSTOM_OP_TCT: blocks the instruction stream until the task
// completion token of one (tile, channel, direction) arrives.
//
// This is the only command that stalls. Queue pushes return immediately and
// their transfers run concurrently, so where these waits sit is what decides
// how much of the graph overlaps: a wait placed between two transfers that do
// not depend on each other throws away parallelism that was free.
//
// Waits one token at a time; awaiting several concurrent transfers takes one
// command each. The transfer must have been armed by issue_token, otherwise no
// token is ever raised and this blocks forever.
class tct_wait : public amd::command
{
public:
    tct_wait() = default;

    void emit(npu::placement const& where, std::vector<word>& out) const override;
};

} // namespace lnpu::nex::amd::commands
