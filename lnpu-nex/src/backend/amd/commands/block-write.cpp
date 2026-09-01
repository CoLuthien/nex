
#include "block-write.hpp"

namespace lnpu::nex::amd::cmds
{
namespace
{
constexpr command::word kOpcode    = 1; // XAIE_IO_BLOCKWRITE
constexpr command::word kOpWords   = 12;
constexpr command::word kBdRegBase = 0x1D000; // shim BD register block

constexpr command::word kColShift = 25;
constexpr command::word kRowShift = 20;
constexpr command::word kBdShift  = 5;

constexpr command::word kPacketEnableShift = 30;
constexpr command::word kOutOfOrderShift   = 24;
constexpr command::word kPacketIdShift     = 19;
constexpr command::word kPacketTypeShift   = 16;

constexpr command::word kDimSizeShift   = 20;
constexpr command::word kDimStrideShift = 0;
constexpr command::word kAxCacheShift   = 24;
constexpr command::word kIterSizeShift  = 20;

constexpr command::word kNextBdShift  = 27;
constexpr command::word kValidBdShift = 25;

// Constant burst-size field the emitter always sets on the D1 word.
constexpr command::word kBurstSize = 0xC0000000;

// Strides and the iteration count are stored biased by -1.
constexpr command::word
biased(command::word value)
{
    return value == 0 ? 0 : value - 1;
}
} // namespace

block_write::block_write(parameters param) //
    : m_param(param)
{
}

void
block_write::emit(npu::placement const& where, std::vector<word>& out) const
{
    out.push_back(kOpcode);
    out.push_back(0);

    // word 2: which BD register block this writes -- tile coordinate plus bd id
    out.push_back((where.location.row << kRowShift)   //
                  | (where.location.col << kColShift) //
                  | (where.bd << kBdShift)            //
                  | kBdRegBase);

    out.push_back(kOpWords * sizeof(word)); // payload size in bytes
    out.push_back(m_param.length);
    out.push_back(m_param.offset);

    // word 6: packet header. Zero unless the design routes by packet id.
    out.push_back((static_cast<word>(m_param.packet_enable) << kPacketEnableShift) //
                  | (m_param.out_of_order_id << kOutOfOrderShift)                  //
                  | (m_param.packet_id << kPacketIdShift)                          //
                  | (m_param.packet_type << kPacketTypeShift));

    // words 7-9: D0, D1, D2. D2 carries no size -- it is implied by length.
    out.push_back((m_param.d0.size << kDimSizeShift) //
                  | (biased(m_param.d0.stride) << kDimStrideShift));
    out.push_back(kBurstSize                           //
                  | (m_param.d1.size << kDimSizeShift) //
                  | (biased(m_param.d1.stride) << kDimStrideShift));
    out.push_back((m_param.cache_flag << kAxCacheShift) //
                  | (biased(m_param.d2.stride) << kDimStrideShift));

    // word 10: iteration dimension
    out.push_back((biased(m_param.iteration_size) << kIterSizeShift) //
                  | biased(m_param.iteration_stride));

    // word 11: chaining and validity. The lock fields the register also carries
    // are unused on NPU2, which synchronises through completion tokens instead.
    out.push_back((m_param.next_bd_id << kNextBdShift) //
                  | (static_cast<word>(m_param.valid_bd) << kValidBdShift));
}

} // namespace lnpu::nex::amd::cmds
