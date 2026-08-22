#include "rmsnorm.hpp"

#include "amd/commands/block-write.hpp"
#include "amd/commands/ddr-patch.hpp"
#include "amd/commands/issue-token.hpp"
#include "amd/commands/queue-push.hpp"
#include "amd/commands/tct-wait.hpp"

#include <array>
#include <memory>
#include <utility>
#include <vector>

namespace lnpu::nex::amd::programs
{
namespace
{

/// Channels one shim column offers in each direction. Two streams per column, per direction.
constexpr std::uint32_t kChannelsPerColumn = 2;

/// AxCache QoS bits every descriptor the compiler emits carries.
constexpr command::word kNormalCache = 0x2;

std::error_code
failure(std::errc code)
{
    return std::make_error_code(code);
}

/**
 * @brief Where the n-th stream of a direction lands.
 *
 * Streams are laid down in order across the shim: two per column, then on to the next. Each
 * direction counts separately, because a column's MM2S and S2MM channels are distinct hardware.
 */
npu::placement
at(rmsnorm::outlet const& outlet, npu::dma_direction direction, npu::bd_id bd)
{
    return npu::placement{
        .location  = {.col = outlet.column, .row = 0},
        .bd        = bd,
        .channel   = outlet.channel == 0 ? npu::it_channel_0 : npu::it_channel_1,
        .direction = direction,
    };
}

/// The arrangement a design takes unless it says otherwise: two channels per column, in order.
rmsnorm::outlet
sequential(std::uint32_t stream)
{
    return {.column = stream / kChannelsPerColumn, .channel = stream % kChannelsPerColumn};
}

rmsnorm::outlet
outlet_of(std::vector<rmsnorm::outlet> const& given, std::uint32_t stream)
{
    return stream < given.size() ? given[stream] : sequential(stream);
}

template <typename T, typename... A>
command::unique
make(A&&... a)
{
    return std::make_unique<T>(std::forward<A>(a)...);
}

} // namespace

rmsnorm::rmsnorm(parameters param) : m_param(std::move(param))
{
}

std::uint32_t
rmsnorm::cores() const
{
    return m_param.design.columns * m_param.channels;
}

std::uint32_t
rmsnorm::slice() const
{
    auto const total = cores();
    return total == 0 ? 0 : m_param.elements / total;
}

std::uint32_t
rmsnorm::buffer_descriptors_used() const
{
    // Reading and writing are separate descriptors even on the same column, and a weight stream
    // occupies one more. The busiest column is the one carrying a full pair of channels of each.
    return kChannelsPerColumn * (m_param.weighted ? 3 : 2);
}

std::error_code
rmsnorm::wire(command_list& sequence) const
{
    auto const& design = m_param.design;

    if (design.columns == 0 or m_param.channels == 0)
    {
        return failure(std::errc::invalid_argument);
    }

    auto const total = cores();

    // An uneven split would leave a core with a different slice than its descriptor claims, and
    // nothing downstream would notice; the run would simply normalize over the wrong extent.
    if (m_param.elements == 0 or m_param.elements % total != 0)
    {
        return failure(std::errc::invalid_argument);
    }

    auto const per_core_bytes = slice() * m_param.element_bytes;

    // Descriptor lengths count 32-bit words, whatever the element type is.
    if (per_core_bytes == 0 or per_core_bytes % sizeof(command::word) != 0)
    {
        return failure(std::errc::invalid_argument);
    }

    auto const words = static_cast<command::word>(per_core_bytes / sizeof(command::word));

    // One weight stream per channel: every channel's group of columns is fed its own copy.
    auto const weight_streams = m_param.weighted ? m_param.channels : 0U;
    auto const reads          = total + weight_streams;

    // Streams spread across the shim until they run out of channels, which is not the same as
    // running out of the columns the design computes on: a one-column design still reaches into
    // the second column's shim when it needs a third stream. The budget that actually binds is
    // the partition's, and it is the one IRON spells as channels * (columns + 1) <= 16.
    auto const available = design.partition_columns * kChannelsPerColumn;

    if (reads > available or total > available)
    {
        return failure(std::errc::result_out_of_range);
    }

    // Descriptors are numbered per column, in the order they are written there.
    std::vector<std::uint32_t> next_bd(design.partition_columns, 0);

    auto const claim = [&next_bd](std::uint32_t column) {
        return static_cast<npu::bd_id>(next_bd[column]++);
    };

    auto const transfer =
        [&](npu::placement const& where, command::word offset, command::word argument) {
            // The offset rides in two places. The descriptor carries it so the transfer starts
            // at the right element, and the patch carries it so XRT adds it to whatever address
            // the argument turns out to have; leaving it out of either one reads from the front
            // of the buffer.
            sequence.record(where,
                            make<cmds::block_write>(cmds::block_write::parameters{
                                .length = words, .offset = offset, .cache_flag = kNormalCache}));
            sequence.record(where,
                            make<commands::ddr_patch>(commands::ddr_patch::parameters{
                                .argument_index = argument, .offset_bytes = offset}));
        };

    // Reads first: every core's slice, then the weight each channel group shares. The weight
    // continues the same channel sequence rather than starting over, which is why a weighted
    // design runs out of shim channels one column sooner than an unweighted one.
    for (std::uint32_t core = 0; core < total; ++core)
    {
        auto const spot  = outlet_of(m_param.reads, core);
        auto const where = at(spot, npu::MM2S, claim(spot.column));

        transfer(where,
                 m_param.input.offset_bytes + core * per_core_bytes,
                 m_param.input.argument_index);
        sequence.record(where, make<commands::queue_push>(commands::queue_push::parameters{}));
    }

    for (std::uint32_t stream = 0; stream < weight_streams; ++stream)
    {
        auto const spot  = outlet_of(m_param.reads, total + stream);
        auto const where = at(spot, npu::MM2S, claim(spot.column));

        // Every group reads the same weight vector from the start; there is nothing to offset.
        transfer(where, m_param.weight.offset_bytes, m_param.weight.argument_index);
        sequence.record(where, make<commands::queue_push>(commands::queue_push::parameters{}));
    }

    // Writes, each announcing completion so that the waits below have something to wait on.
    for (std::uint32_t core = 0; core < total; ++core)
    {
        auto const spot  = outlet_of(m_param.writes, core);
        auto const where = at(spot, npu::S2MM, claim(spot.column));

        transfer(where,
                 m_param.output.offset_bytes + core * per_core_bytes,
                 m_param.output.argument_index);

        // The token field has to be set before the descriptor is queued, not after.
        sequence.record(where, make<commands::issue_token>(commands::issue_token::parameters{}));
        sequence.record(
            where,
            make<commands::queue_push>(commands::queue_push::parameters{.issue_token = true}));
    }

    // Waits come last, all of them: holding one back until its own write is queued would stall
    // the writes behind it for no reason.
    for (std::uint32_t core = 0; core < total; ++core)
    {
        sequence.record(at(outlet_of(m_param.writes, core), npu::S2MM, npu::bd_0),
                        make<commands::tct_wait>());
    }

    return {};
}

} // namespace lnpu::nex::amd::programs
