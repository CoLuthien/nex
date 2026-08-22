
#include "queue-push.hpp"

namespace lnpu::nex::amd::commands
{
namespace
{
constexpr command::word kOpcode  = 0; // XAIE_IO_WRITE
constexpr command::word kOpWords = 6;

// Task queue register of channel 0 in the S2MM direction. The other three
// queues are reached by setting the direction bit and stepping one channel.
constexpr command::word kQueueRegBase  = 0x1D204;
constexpr command::word kDirectionMm2s = 0x10;
constexpr command::word kChannelStride = 0x08;

constexpr command::word kColShift = 25;
constexpr command::word kRowShift = 20;

constexpr command::word kBdIdMask        = 0xF;
constexpr command::word kRepeatShift     = 16;
constexpr command::word kRepeatMask      = 0xFF;
constexpr command::word kIssueTokenShift = 31;

command::word
queue_register(npu::placement const& where)
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

    return reg;
}
} // namespace

queue_push::queue_push(parameters param) //
    : m_param(param)
{
}

void
queue_push::emit(npu::placement const& where, std::vector<word>& out) const
{
    out.push_back(kOpcode);
    out.push_back(0);

    // word 2: the task queue register this write targets
    out.push_back((where.location.row << kRowShift)   //
                  | (where.location.col << kColShift) //
                  | queue_register(where));

    out.push_back(0);

    // word 4: the queue entry itself -- which BD to run, how often, and whether
    // finishing it raises a completion token.
    out.push_back((static_cast<word>(where.bd) & kBdIdMask)                //
                  | ((m_param.repeat_count & kRepeatMask) << kRepeatShift) //
                  | (static_cast<word>(m_param.issue_token) << kIssueTokenShift));

    out.push_back(kOpWords * sizeof(word)); // payload size in bytes
}

} // namespace lnpu::nex::amd::commands
