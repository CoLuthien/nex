#include "gemm.hpp"

#include "amd/descriptor.hpp"
#include "amd/program-registry.hpp"

#include "amd/commands/block-write.hpp"
#include "amd/commands/ddr-patch.hpp"
#include "amd/commands/issue-token.hpp"
#include "amd/commands/queue-push.hpp"
#include "amd/commands/register-write.hpp"
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

constexpr std::uint32_t kChannelsPerColumn = 2;
constexpr command::word kNormalCache       = 0x2;

/// The widest stride a descriptor can hold, in 32-bit words.
constexpr std::uint64_t kWidestStride = (1U << 20);

/// A dimension walked only once has nothing to advance by, and the compiler leaves its stride at
/// rest rather than at whatever the advance would have been. This applies to the iteration
/// dimension and to the innermost strides alike.
command::word
advance(std::uint32_t iterations, std::uint64_t stride)
{
    return iterations > 1 ? static_cast<command::word>(stride) : 1;
}

std::error_code
failure(std::errc code)
{
    return std::make_error_code(code);
}

template <typename T, typename... A>
command::unique
make(A&&... a)
{
    return std::make_unique<T>(std::forward<A>(a)...);
}

npu::placement
at(std::uint32_t column, std::uint32_t channel, npu::dma_direction direction, npu::bd_id bd)
{
    return npu::placement{
        .location  = {.col = column, .row = 0},
        .bd        = bd,
        .channel   = channel == 0 ? npu::it_channel_0 : npu::it_channel_1,
        .direction = direction,
    };
}

} // namespace

gemm::design
gemm::describe(descriptor const& metadata)
{
    if (metadata.op() != kOp)
    {
        throw std::runtime_error("[amd::gemm] this design is '" + std::string{metadata.op()} +
                                 "', not '" + std::string{kOp} + "'");
    }

    return design{
        .common         = metadata.common(),
        .rows           = metadata.u32("rows"),
        .first_core_row = metadata.u32("first_core_row"),
        .tile_m         = metadata.u32("tile", "m"),
        .tile_k         = metadata.u32("tile", "k"),
        .tile_n         = metadata.u32("tile", "n"),
        .element_bytes  = metadata.u32("element_bytes"),
    };
}

std::shared_ptr<gemm>
gemm::lower(descriptor const& metadata, layer_description const& layer, std::error_code& ec)
{
    ec = {};

    auto const refuse = [&ec, &layer](std::errc code, std::string_view why) {
        spdlog::error("[amd::gemm] '{}' is not a product this design runs: {}", layer.name(), why);
        ec = std::make_error_code(code);
        return nullptr;
    };

    // A bias would have to be added after the last accumulation, and there is no stream carrying
    // it -- the design's third argument is where C is written, not read.
    if (layer.input_count() > 2)
    {
        return refuse(std::errc::not_supported,
                      "it has a bias input, which the design has no stream for");
    }

    auto const* left  = shape_of(layer.input(0));
    auto const* right = shape_of(layer.input(1));
    if (nullptr == left or nullptr == right)
    {
        return refuse(std::errc::invalid_argument,
                      "its operands carry no shape; the graph was not shape-inferred");
    }

    // Anything else is a batch of products, which is more descriptors than are emitted here.
    if (left->rank() != 2 or right->rank() != 2)
    {
        return refuse(std::errc::not_supported, "only the two-dimensional product is emitted here");
    }

    if (not elements_in(*left) or not elements_in(*right))
    {
        return refuse(std::errc::not_supported,
                      "one of its extents is dynamic, and a descriptor needs a number");
    }

    auto const rows    = left->extent(0);
    auto const shared  = left->extent(1);
    auto const columns = right->extent(1);

    // K is what both operands agree on, and a graph that disagrees with itself would have one of
    // the two streams walking off the end of its buffer.
    if (shared != right->extent(0))
    {
        return refuse(std::errc::invalid_argument, "its operands disagree about K");
    }

    // The A stream reads rows as they are stored and B is streamed as [k, n]; a transposed operand
    // is a different set of descriptors, not a different number in these.
    for (auto const* which : {"transA", "transB"})
    {
        auto const* transposed = layer.attribute_as<std::int64_t>(which);
        if (nullptr != transposed and *transposed != 0)
        {
            return refuse(std::errc::not_supported,
                          "an operand is transposed, which is another design");
        }
    }

    // Nothing in the array scales what it accumulated, so a scale that is not 1 would be dropped
    // silently -- which is the one outcome worth refusing over.
    for (auto const* which : {"alpha", "beta"})
    {
        auto const* scale = layer.attribute_as<float>(which);
        if (nullptr != scale and *scale != 1.0F)
        {
            return refuse(std::errc::not_supported,
                          "it scales its product, and the design does not");
        }
    }

    design     fixed{};
    parameters param{};
    try
    {
        fixed = describe(metadata);

        param.a = {.argument_index = metadata.argument("a")};
        param.b = {.argument_index = metadata.argument("b")};
        param.c = {.argument_index = metadata.argument("c")};
    }
    catch (std::exception const& thrown)
    {
        // describe() and argument() throw at a mismatch between this program and the xclbin it
        // was handed, which is the caller's mistake and not this layer's. It still has to come
        // back as an error rather than unwind a walk over a whole graph.
        spdlog::error("[amd::gemm] '{}': {}", layer.name(), thrown.what());
        ec = std::make_error_code(std::errc::invalid_argument);
        return nullptr;
    }

    param.m = static_cast<std::uint32_t>(rows);
    param.k = static_cast<std::uint32_t>(shared);
    param.n = static_cast<std::uint32_t>(columns);

    // A network runs one layer after another against a single hardware context, which is the case
    // this flag exists for: the A and B streams of the run before this one have to have drained
    // before this one changes what they carry.
    param.wait_for_inputs = true;

    return std::make_shared<gemm>(std::move(fixed), std::move(param));
}

gemm::gemm(design fixed, parameters param) : m_design(std::move(fixed)), m_param(std::move(param))
{
}

std::uint32_t
gemm::k_steps() const
{
    return m_design.tile_k == 0 ? 0 : m_param.k / m_design.tile_k;
}

std::uint32_t
gemm::tile_steps() const
{
    auto const block = m_design.tile_m * m_design.rows;
    auto const wide  = m_design.tile_n * m_design.common.columns;
    if (block == 0 or wide == 0) return 0;
    return (m_param.m / block) * (m_param.n / wide);
}

std::uint32_t
gemm::buffer_descriptors_used() const
{
    // A column carries its slice of C, its slice of B, and on some columns a share of A.
    return 3;
}

std::error_code
gemm::wire(command_list& sequence) const
{
    auto const& common = m_design.common;

    auto const columns = common.columns;
    auto const rows    = m_design.rows;
    auto const bytes   = m_design.element_bytes;

    if (columns == 0 or rows == 0 or bytes == 0)
    {
        return failure(std::errc::invalid_argument);
    }
    if (m_design.tile_m == 0 or m_design.tile_k == 0 or m_design.tile_n == 0)
    {
        return failure(std::errc::invalid_argument);
    }

    auto const block = m_design.tile_m * rows;    // rows of C the array holds at once
    auto const wide  = m_design.tile_n * columns; // columns of C the array holds at once

    // A shape that does not divide leaves some core computing a tile that is partly outside the
    // matrix, which the descriptors would happily fetch.
    if (m_param.m == 0 or m_param.m % block != 0)
    {
        return failure(std::errc::invalid_argument);
    }
    if (m_param.k == 0 or m_param.k % m_design.tile_k != 0)
    {
        return failure(std::errc::invalid_argument);
    }

    if (m_param.n == 0 or m_param.n % wide != 0) return failure(std::errc::invalid_argument);

    if (columns > common.partition_columns) return failure(std::errc::result_out_of_range);
    if (common.parameter_slots.size() < 2) return failure(std::errc::invalid_argument);

    // Descriptor extents count 32-bit words whatever the element type is.
    auto const per_word = sizeof(command::word) / bytes;
    if (per_word == 0 or sizeof(command::word) % bytes != 0)
        return failure(std::errc::invalid_argument);

    auto const words = [per_word](std::uint64_t elements) -> command::word {
        return static_cast<command::word>(elements / per_word);
    };

    auto const m_steps = m_param.m / block;
    auto const n_steps = m_param.n / wide;

    // More rows than the array holds at once is a shape the compiler does emit for, but it does
    // not do it by iterating what is here: it describes A and C once per band of rows, so the
    // stream grows a descriptor per band rather than reusing one. Emitting the single-band form
    // for a taller matrix would compute only its first block and say nothing about it, so it is
    // refused until themulti-band form is written. Gemma4 does not reach it -- a 128-row prefill on
    // a 32x4 array is exactly one band.
    if (m_steps != 1) return failure(std::errc::not_supported);

    // B walks its column blocks with a stride that grows with K, and the field it goes in is 20
    // bits wide. Gemma4's 6144x1536 down-projection is past it; aiecc refuses such a design
    // outright, and there is no honest way to emit for it here either -- the product has to be
    // split along K and accumulated, which is the caller's decision to make.
    auto const b_iteration_stride =
        static_cast<std::uint64_t>(m_param.k) * m_design.tile_n * columns / per_word;
    if (b_iteration_stride >= kWidestStride) return failure(std::errc::value_too_large);

    // ---- tell every core how many times to go round, then let them go
    for (std::uint32_t row = 0; row < rows; ++row)
    {
        for (std::uint32_t column = 0; column < columns; ++column)
        {
            npu::placement const core{
                .location = {.col = column, .row = m_design.first_core_row + row}};

            for (std::uint32_t slot = 0; slot < 2; ++slot)
            {
                sequence.record(core,
                                make<commands::register_write>(commands::register_write::parameters{
                                    .address = common.parameter_slots[slot],
                                    .value   = slot == 0 ? k_steps() : tile_steps()}));
            }
        }
    }

    for (std::uint32_t row = 0; row < rows; ++row)
    {
        for (std::uint32_t column = 0; column < columns; ++column)
        {
            sequence.record(
                npu::placement{.location = {.col = column, .row = m_design.first_core_row + row}},
                make<commands::register_write>(commands::register_write::parameters{
                    .address = common.start_register, .value = 1}));
        }
    }

    // ---- the transfers
    std::vector<std::uint32_t> next_bd(columns, 0);
    std::vector<std::uint32_t> next_read_channel(columns, 0);

    auto const claim = [&next_bd](std::uint32_t column) {
        return static_cast<npu::bd_id>(next_bd[column]++);
    };

    // The offset goes in twice: into the descriptor, so the transfer starts at the right
    // element, and into the patch, so XRT adds it to whatever address the argument turns out to
    // have. Leaving it out of either one reads from the front of the buffer.
    auto const describe = [&](npu::placement const&         where,
                              cmds::block_write::parameters how,
                              binding const&                what,
                              command::word                 offset) {
        how.offset = offset;
        sequence.record(where, make<cmds::block_write>(how));
        sequence.record(where,
                        make<commands::ddr_patch>(commands::ddr_patch::parameters{
                            .argument_index = what.argument_index,
                            .offset_bytes   = what.offset_bytes + offset}));
    };

    // Columns are visited once, and each one's results, its share of A and its slice of B are
    // described together. A does not need a stream per column: a band of it feeds a whole row of
    // cores across the array, so there are as many streams as core rows -- or as many as there
    // are columns to put them on, whichever is fewer, in which case one stream feeds several
    // rows and carries their bands back to back.
    auto const a_streams = rows < columns ? rows : columns;
    auto const a_every   = columns / a_streams;
    auto const a_bands   = rows / a_streams;
    auto const a_height  = a_bands * m_design.tile_m;

    // Where every input stream was put, so that each can be waited on once the results are in.
    // Left empty when nothing is waiting on them; see parameters::wait_for_inputs.
    std::vector<npu::placement> reading{};

    // Queues an input stream. A transfer only produces a token if its queue register was told to
    // before the descriptor was pushed, so the two go together or neither does.
    auto const announce = [&](npu::placement const& where) {
        if (m_param.wait_for_inputs)
        {
            sequence.record(where,
                            make<commands::issue_token>(commands::issue_token::parameters{}));
            reading.push_back(where);
        }

        sequence.record(where,
                        make<commands::queue_push>(commands::queue_push::parameters{
                            .repeat_count = n_steps - 1, .issue_token = m_param.wait_for_inputs}));
    };

    // The array is walked once per column of it, but a step is not a column: C and B belong to
    // the column of that index while A, being fewer, belongs to a column further along. The
    // compiler describes them in that interleaved order, and since descriptor numbers are handed
    // out per column as they are written, the order is what decides them.
    for (std::uint32_t step = 0; step < columns; ++step)
    {
        auto const column = step;

        // C: this column's stripe of the result, tile_n wide, gathered every wide columns.
        {
            auto const bd    = claim(column);
            auto const where = at(column, 0, npu::S2MM, bd);

            describe(where,
                     {.length = words(static_cast<std::uint64_t>(m_param.m) * m_param.n / columns),
                      .d0     = {.size = words(m_design.tile_n), .stride = 1},
                      .d1     = {.size = m_param.m, .stride = words(m_param.n)},
                      .d2     = {.stride = advance(n_steps, words(wide))},
                      .iteration_size = m_steps,
                      .iteration_stride =
                          advance(m_steps, words(static_cast<std::uint64_t>(block) * m_param.n)),
                      .cache_flag = kNormalCache},
                     m_param.c,
                     words(m_design.tile_n) * sizeof(command::word) * column);

            sequence.record(where,
                            make<commands::issue_token>(commands::issue_token::parameters{}));
            sequence.record(where,
                            make<commands::queue_push>(commands::queue_push::parameters{
                                .repeat_count = m_steps - 1, .issue_token = true}));
        }

        // A: one core row's band of the left operand, read again for every tile of C along N.
        if (step < a_streams)
        {
            auto const band  = step;
            auto const host  = step * a_every;
            auto const bd    = claim(host);
            auto const where = at(host, next_read_channel[host]++, npu::MM2S, bd);

            describe(where,
                     {.length     = words(static_cast<std::uint64_t>(a_height) * m_param.k),
                      .d0         = {.size = words(m_design.tile_k), .stride = 1},
                      .d1         = {.size = a_height, .stride = words(m_param.k)},
                      .d2         = {.stride = advance(k_steps(), words(m_design.tile_k))},
                      .cache_flag = kNormalCache},
                     m_param.a,
                     static_cast<command::word>(static_cast<std::uint64_t>(band) * a_height *
                                                m_param.k * bytes));

            announce(where);
        }

        // B: this column's slice of the right operand, advancing one slice per tile along N.
        {
            auto const bd    = claim(column);
            auto const where = at(column, next_read_channel[column]++, npu::MM2S, bd);

            describe(where,
                     {.length = words(static_cast<std::uint64_t>(m_param.k) * m_design.tile_n),
                      .d0     = {.size = words(m_design.tile_k), .stride = 1},
                      .d1     = {.size = m_design.tile_n, .stride = words(m_param.k)},
                      .d2     = {.stride = advance(k_steps(), words(m_design.tile_k))},
                      .iteration_size   = n_steps,
                      .iteration_stride = advance(n_steps, b_iteration_stride),
                      .cache_flag       = kNormalCache},
                     m_param.b,
                     static_cast<command::word>(static_cast<std::uint64_t>(column) * m_param.k *
                                                m_design.tile_n * bytes));

            announce(where);
        }
    }

    for (std::uint32_t column = 0; column < columns; ++column)
    {
        sequence.record(at(column, 0, npu::S2MM, npu::bd_0), make<commands::tct_wait>());
    }

    // The results are in; now make sure nothing is still moving. The descriptor a wait names does
    // not matter -- a token says a channel finished, not which descriptor did -- so bd_0 stands
    // for all of them, exactly as the result waits above use it.
    for (auto const& where : reading)
    {
        sequence.record(npu::placement{.location  = where.location,
                                       .bd        = npu::bd_0,
                                       .channel   = where.channel,
                                       .direction = where.direction},
                        make<commands::tct_wait>());
    }

    return {};
}

} // namespace lnpu::nex::amd::programs
