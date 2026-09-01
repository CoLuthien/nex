
#include "ddr-patch.hpp"

namespace lnpu::nex::amd::commands
{
namespace
{
constexpr command::word kOpcode  = 0x81; // XAIE_IO_CUSTOM_OP_DDR_PATCH
constexpr command::word kOpWords = 12;

// The patch targets the address register of the BD block, four bytes past the
// block base that block_write addresses.
constexpr command::word kBdAddressReg = 0x1D004;

constexpr command::word kColShift = 25;
constexpr command::word kRowShift = 20;
constexpr command::word kBdShift  = 5;
} // namespace

ddr_patch::ddr_patch(parameters param) //
    : m_param(param)
{
}

void
ddr_patch::emit(npu::placement const& where, std::vector<word>& out) const
{
    out.push_back(kOpcode);
    out.push_back(kOpWords * sizeof(word)); // payload size in bytes

    // words 2-5 are unused by this operation
    out.push_back(0);
    out.push_back(0);
    out.push_back(0);
    out.push_back(0);

    // word 6: the BD address register to patch
    out.push_back((where.location.col << kColShift)   //
                  | (where.location.row << kRowShift) //
                  | (where.bd << kBdShift)            //
                  | kBdAddressReg);

    out.push_back(0);
    out.push_back(m_param.argument_index); // word 8
    out.push_back(0);
    out.push_back(m_param.offset_bytes); // word 10
    out.push_back(0);
}

} // namespace lnpu::nex::amd::commands
