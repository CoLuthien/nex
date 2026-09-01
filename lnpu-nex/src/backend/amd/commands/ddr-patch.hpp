
#pragma once

#include "amd/command.hpp"

namespace lnpu::nex::amd::commands
{
/** AI Asissted code
 */

// XAIE_IO_CUSTOM_OP_DDR_PATCH: tells the driver to fix up the DDR address held
// by a buffer descriptor at submission time.
//
// An instruction stream is built before any device buffer exists, so a BD
// cannot carry an address. It carries a reference instead -- argument index
// plus a byte offset -- and the driver resolves it to the address of the buffer
// object bound to that kernel argument. This is what lets one arena buffer back
// many logical tensors: every BD points at the same argument and differs only
// in the offset the memory planner assigned.
//
// Must follow the block_write that configures the same (tile, bd).
class ddr_patch : public amd::command
{
public:
    struct parameters
    {
        word argument_index{}; // kernel argument holding the buffer, 0-based
        word offset_bytes{};   // byte offset into that argument
    };

    explicit ddr_patch(parameters param);

    void emit(npu::placement const& where, std::vector<word>& out) const override;

private:
    parameters m_param;
};

} // namespace lnpu::nex::amd::commands
