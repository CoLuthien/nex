
#pragma once

#include "amd/command.hpp"

namespace lnpu::nex::amd::commands
{

/** AI Asissted code
 */

// XAIE_IO_MASKWRITE against a shim DMA task queue: arms the queue so that
// completing a transfer raises a task completion token.
//
// Only a transfer whose queue has been armed can be awaited by tct_wait, so
// this precedes the queue_push of any transfer the host needs to synchronise
// on -- in practice the device-to-host direction, which carries the result.
//
// The write is masked because it touches only the token control field of a
// register the queue shares with other state.
class issue_token : public amd::command
{
public:
    struct parameters
    {
        // Packet id of the shim controller that reports the token. The value
        // the mlir-aie runtime uses is 15; it is not derived from anything the
        // graph knows, so it is carried here rather than computed.
        word controller_packet_id{15};
    };

    // A nested type's default member initializer cannot be used in a default
    // argument of the enclosing class, so the no-argument form is its own
    // constructor rather than a defaulted parameter.
    issue_token() = default;
    explicit issue_token(parameters param);

    void emit(npu::placement const& where, std::vector<word>& out) const override;

private:
    parameters m_param{};
};

} // namespace lnpu::nex::amd::commands
