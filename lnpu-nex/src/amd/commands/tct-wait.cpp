
#include "tct-wait.hpp"

namespace lnpu::nex::amd::commands
{
namespace
{
constexpr command::word kOpcode  = 0x80; // XAIE_IO_CUSTOM_OP_TCT
constexpr command::word kOpWords = 4;

// This command packs the tile coordinate differently from the BD commands:
// row and col sit low, not at bits 20 and 25.
constexpr command::word kRowShift       = 8;
constexpr command::word kColShift       = 16;
constexpr command::word kDirectionShift = 0;
constexpr command::word kChannelShift   = 24;

// Fixed in every sequence mlir-aie emits; its meaning is not documented, but
// omitting it makes the wait malformed.
constexpr command::word kWaitConstant = 0x10100;
} // namespace

void
tct_wait::emit(npu::placement const& where, std::vector<word>& out) const
{
    out.push_back(kOpcode);
    out.push_back(kOpWords * sizeof(word)); // payload size in bytes

    out.push_back((where.location.row << kRowShift)   //
                  | (where.location.col << kColShift) //
                  | (static_cast<word>(where.direction) << kDirectionShift));

    out.push_back((static_cast<word>(where.channel) << kChannelShift) | kWaitConstant);
}

} // namespace lnpu::nex::amd::commands
