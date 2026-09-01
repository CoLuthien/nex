
#include "issue-token.hpp"

namespace lnpu::nex::amd::commands
{
namespace
{
constexpr command::word kOpcode  = 3; // XAIE_IO_MASKWRITE
constexpr command::word kOpWords = 7;

// Task queue register of channel 0 in the S2MM direction. queue_push targets
// the same block four bytes further in.
constexpr command::word kQueueRegBase  = 0x1D200;
constexpr command::word kDirectionMm2s = 0x10;
constexpr command::word kChannelStride = 0x08;

constexpr command::word kColShift      = 25;
constexpr command::word kRowShift      = 20;
constexpr command::word kPacketIdShift = 8;

// Selects the token control field; the rest of the register is left alone.
constexpr command::word kTokenFieldMask = 0x00001F00;
} // namespace

issue_token::issue_token(parameters param) //
    : m_param(param)
{
}

void
issue_token::emit(npu::placement const& where, std::vector<word>& out) const
{
    command::word reg = kQueueRegBase;

    if (npu::MM2S == where.direction)
    {
        reg |= kDirectionMm2s;
    }

    if (npu::it_channel_1 == where.channel)
    {
        reg += kChannelStride;
    }

    out.push_back(kOpcode);
    out.push_back(0);

    // word 2: the queue register whose token control field is being set
    out.push_back((where.location.row << kRowShift)   //
                  | (where.location.col << kColShift) //
                  | reg);

    out.push_back(0);
    out.push_back(m_param.controller_packet_id << kPacketIdShift); // word 4: value
    out.push_back(kTokenFieldMask);                                // word 5: mask
    out.push_back(kOpWords * sizeof(word));                        // payload size in bytes
}

} // namespace lnpu::nex::amd::commands
