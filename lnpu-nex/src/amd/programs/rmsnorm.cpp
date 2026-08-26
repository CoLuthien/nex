#include "rmsnorm.hpp"

#include "amd/descriptor.hpp"
#include "amd/program-registry.hpp"

#include "amd/commands/block-write.hpp"
#include "amd/commands/ddr-patch.hpp"
#include "amd/commands/issue-token.hpp"
#include "amd/commands/queue-push.hpp"
#include "amd/commands/tct-wait.hpp"

#include <spdlog/spdlog.h>

#include <memory>
#include <stdexcept>
#include <string>
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
 * This is our arrangement, not one read off the design -- nothing above the shim can tell which
 * channel filled a fifo, so the only thing it has to be is consistent within a stream.
 */
npu::placement
at(std::uint32_t stream, npu::dma_direction direction, npu::bd_id bd)
{
    return npu::placement{
        .location  = {.col = stream / kChannelsPerColumn, .row = 0},
        .bd        = bd,
        .channel   = stream % kChannelsPerColumn == 0 ? npu::it_channel_0 : npu::it_channel_1,
        .direction = direction,
    };
}

/// Which shim column carries the n-th stream, for claiming descriptors on it.
constexpr std::uint32_t
column_of(std::uint32_t stream)
{
    return stream / kChannelsPerColumn;
}

template <typename T, typename... A>
command::unique
make(A&&... a)
{
    return std::make_unique<T>(std::forward<A>(a)...);
}

} // namespace

rmsnorm::design
rmsnorm::describe(descriptor const& metadata)
{
    if (metadata.op() != kOp)
    {
        throw std::runtime_error("[amd::rmsnorm] this design is '" + std::string{metadata.op()} +
                                 "', not '" + std::string{kOp} + "'");
    }

    return design{
        .common        = metadata.common(),
        .channels      = metadata.u32("channels"),
        .tile          = metadata.u32("tile"),
        .elements      = metadata.u32("elements"),
        .weighted      = metadata.flag("weighted"),
        .element_bytes = metadata.u32("element_bytes"),
    };
}

std::shared_ptr<rmsnorm>
rmsnorm::lower(descriptor const& metadata, layer_description const& layer, std::error_code& ec)
{
    ec = {};

    auto const refuse = [&ec, &layer](std::errc code, std::string const& why) {
        spdlog::error(
            "[amd::rmsnorm] '{}' is not a run this design normalizes: {}", layer.name(), why);
        ec = std::make_error_code(code);
        return nullptr;
    };

    auto const* input = shape_of(layer.input(0));
    if (nullptr == input)
    {
        return refuse(std::errc::invalid_argument,
                      "its input carries no shape; the graph was not shape-inferred");
    }

    // Everything from `axis` onwards is one normalized group; what comes before it is a count of
    // such groups. The design normalizes a group per core pass, so the two have to be told apart
    // here rather than left as one total.
    auto const  rank   = static_cast<std::int64_t>(input->rank());
    auto const* stated = layer.attribute_as<std::int64_t>("axis");

    // onnx counts a negative axis from the end, and -1 is the default.
    auto axis = nullptr == stated ? -1 : *stated;
    if (axis < 0) axis += rank;

    if (axis < 0 or axis >= rank)
    {
        return refuse(std::errc::invalid_argument, "its axis is outside the input's rank");
    }

    auto const total = elements_in(*input);
    auto const group = elements_in(*input, static_cast<lnpu::layout::rank_type>(axis));
    if (not total or not group)
    {
        return refuse(std::errc::not_supported,
                      "one of its extents is dynamic, and a descriptor needs a number");
    }

    design     fixed{};
    parameters param{};
    try
    {
        fixed = describe(metadata);

        // A weight stream is either baked in or it is not, and a design without one would ignore
        // a scale the graph asked for while a design with one would stream whatever the argument
        // happened to be bound to. Neither says anything at run time. This is also what makes
        // input(1) below a tensor that exists, so it is asked before anything names one.
        auto const scaled = layer.input_count() > 1;
        if (scaled != fixed.weighted)
        {
            return refuse(
                std::errc::invalid_argument,
                fixed.weighted
                    ? "the design was baked with a weight stream and the layer has no scale"
                    : "the layer has a scale and the design was baked without a weight stream");
        }

        // Which of the node's tensors each argument is, is the piece only this function knows.
        param.input  = {.argument_index = metadata.argument("input"),
                        .tensor         = std::string{layer.input_name(0)}};
        param.output = {.argument_index = metadata.argument("output"),
                        .tensor         = std::string{layer.output_name(0)}};
        if (fixed.weighted)
        {
            param.weight = {.argument_index = metadata.argument("weight"),
                            .tensor         = std::string{layer.input_name(1)}};
        }
    }
    catch (std::exception const& thrown)
    {
        // describe() and argument() throw at a mismatch between this program and the xclbin it
        // was handed, which is the caller's mistake and not this layer's. It still has to come
        // back as an error rather than unwind a walk over a whole graph.
        spdlog::error("[amd::rmsnorm] '{}': {}", layer.name(), thrown.what());
        ec = std::make_error_code(std::errc::invalid_argument);
        return nullptr;
    }

    // wire() refuses a total the design was not baked for, but the group is the one this can
    // check and it cannot: a core normalizes `tile` elements per pass, so a hidden size that is
    // not the tile divides the run into the wrong groups and every one of them comes back
    // normalized by the wrong denominator. Nothing downstream can see that.
    if (*group != fixed.tile)
    {
        return refuse(std::errc::invalid_argument,
                      "this xclbin normalizes groups of " + std::to_string(fixed.tile) +
                          " elements, and the layer's are " + std::to_string(*group));
    }

    param.elements = static_cast<std::uint32_t>(*total);

    return std::make_shared<rmsnorm>(std::move(fixed), std::move(param));
}

rmsnorm::rmsnorm(design fixed, parameters param)
    : m_design(std::move(fixed)), m_param(std::move(param))
{
}

std::uint32_t
rmsnorm::cores() const
{
    return m_design.common.columns * m_design.channels;
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
    return kChannelsPerColumn * (m_design.weighted ? 3 : 2);
}

std::vector<binding>
rmsnorm::bindings() const
{
    if (m_design.weighted) return {m_param.input, m_param.weight, m_param.output};
    return {m_param.input, m_param.output};
}

std::error_code
rmsnorm::wire(command_list& sequence) const
{
    auto const& common = m_design.common;

    if (common.columns == 0 or m_design.channels == 0 or m_design.element_bytes == 0)
    {
        return failure(std::errc::invalid_argument);
    }

    auto const total = cores();

    // The design was baked for one length, and this is where a caller that reached for the wrong
    // xclbin finds out. Running anyway would normalize the front of the buffer and leave the rest
    // as it was, which is not a failure anyone downstream can see.
    if (m_param.elements != m_design.elements)
    {
        return failure(std::errc::invalid_argument);
    }

    // An uneven split would leave a core with a different slice than its descriptor claims, and
    // nothing downstream would notice; the run would simply normalize over the wrong extent.
    if (m_param.elements == 0 or m_param.elements % total != 0)
    {
        return failure(std::errc::invalid_argument);
    }

    auto const per_core_bytes = slice() * m_design.element_bytes;

    // Descriptor lengths count 32-bit words, whatever the element type is.
    if (per_core_bytes == 0 or per_core_bytes % sizeof(command::word) != 0)
    {
        return failure(std::errc::invalid_argument);
    }

    auto const words = static_cast<command::word>(per_core_bytes / sizeof(command::word));

    // One weight stream per channel: every channel's group of columns is fed its own copy.
    auto const weight_streams = m_design.weighted ? m_design.channels : 0U;
    auto const reads          = total + weight_streams;

    // Streams spread across the shim until they run out of channels, which is not the same as
    // running out of the columns the design computes on: a one-column design still reaches into
    // the second column's shim when it needs a third stream. The budget that actually binds is
    // the partition's, and it is the one IRON spells as channels * (columns + 1) <= 16.
    auto const available = common.partition_columns * kChannelsPerColumn;

    if (reads > available or total > available)
    {
        return failure(std::errc::result_out_of_range);
    }

    // Descriptors are numbered per column, in the order they are written there.
    std::vector<std::uint32_t> next_bd(common.partition_columns, 0);

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
        auto const where = at(core, npu::MM2S, claim(column_of(core)));

        transfer(where,
                 m_param.input.offset_bytes + core * per_core_bytes,
                 m_param.input.argument_index);
        sequence.record(where, make<commands::queue_push>(commands::queue_push::parameters{}));
    }

    for (std::uint32_t stream = 0; stream < weight_streams; ++stream)
    {
        auto const at_stream = total + stream;
        auto const where     = at(at_stream, npu::MM2S, claim(column_of(at_stream)));

        // Every group reads the same weight vector from the start; there is nothing to offset.
        transfer(where, m_param.weight.offset_bytes, m_param.weight.argument_index);
        sequence.record(where, make<commands::queue_push>(commands::queue_push::parameters{}));
    }

    // Writes, each announcing completion so that the waits below have something to wait on.
    for (std::uint32_t core = 0; core < total; ++core)
    {
        auto const where = at(core, npu::S2MM, claim(column_of(core)));

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
        sequence.record(at(core, npu::S2MM, npu::bd_0), make<commands::tct_wait>());
    }

    return {};
}

} // namespace lnpu::nex::amd::programs
