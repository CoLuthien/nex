
#include "cases.hpp"

#include "amd/descriptor.hpp"
#include "amd/device.hpp"
#include "amd/operation.hpp"
#include "amd/programs/gemm.hpp"

#include <chrono>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>

namespace hw
{
namespace
{

using namespace lnpu::nex::amd;

/**
 * @brief How far the result may sit from the reference, as a fraction of the result.
 *
 * Judged on the relative L2 norm over the whole product, not element by element. A dot product of
 * signed terms is far smaller than the terms it sums -- with K of 1536 and operands in [-1, 1),
 * the answer runs around 13 while the absolute values it came from sum to about 500 -- so the
 * error left by rounding is roughly the same size everywhere, and an output that happens to land
 * near zero shows an enormous relative error while being perfectly good. Measured on hardware:
 * absolute errors clustered at 0.06 to 0.16 whatever the output was, which per element reads as
 * anything from 0.4% to 5000x and as a norm reads as under a percent.
 *
 * The bound is 2%, twice what the largest correct shape measured. What is left grows with K --
 * 0.00887 at 1536, 0.00950 at 2048, 0.01057 at 3072, which is K to the quarter -- so a bound at
 * the measurement would have failed the next shape up for being slightly less precise rather
 * than wrong.
 *
 * A wrong layout does not slip through a norm. It misses by an order one, not by a last place.
 */
constexpr double kRelativeL2 = 0.02;

/// Per-element bounds, reported but not judged on. See kRelativeL2.
constexpr float kRelativeTolerance = 0.02F;
constexpr float kAbsoluteTolerance = 1e-3F;

/// The operands, in the layout the array reads them from.
struct operands
{
    std::vector<std::uint16_t> a; ///< [m][k], row major.
    std::vector<std::uint16_t> b; ///< [n][k], row major -- the design is b_col_maj.
    std::vector<std::uint16_t> c; ///< [m][n], row major.
};

/**
 * @brief Makes the operands and works out the answer.
 *
 * The reference reads the same stored bf16 the device is handed, not the floats they came from,
 * so operand rounding cancels out and what is left to compare is the arithmetic.
 */
operands
compute(gemm_case const& shape)
{
    operands made{};
    made.a.resize(static_cast<std::size_t>(shape.m) * shape.k);
    made.b.resize(static_cast<std::size_t>(shape.n) * shape.k);
    made.c.resize(static_cast<std::size_t>(shape.m) * shape.n);

    // Seeded off the shape, so every case gets different numbers and the same case gets the same
    // ones on every machine.
    stream_of_numbers numbers{0x5EEDULL ^ (std::uint64_t{shape.m} << 40) ^
                              (std::uint64_t{shape.k} << 20) ^ shape.n};
    numbers.fill(made.a);
    numbers.fill(made.b);

    for (std::uint32_t row = 0; row < shape.m; ++row)
    {
        auto const* left = made.a.data() + static_cast<std::size_t>(row) * shape.k;

        for (std::uint32_t col = 0; col < shape.n; ++col)
        {
            // B's row is this column of the product, which is what b_col_maj buys: both operands
            // are walked contiguously along k.
            auto const* right = made.b.data() + static_cast<std::size_t>(col) * shape.k;

            float sum = 0.0F;
            for (std::uint32_t at = 0; at < shape.k; ++at)
            {
                sum += from_bf16(left[at]) * from_bf16(right[at]);
            }

            made.c[static_cast<std::size_t>(row) * shape.n + col] = to_bf16(sum);
        }
    }

    return made;
}

std::string
label_of(gemm_case const& shape)
{
    return "M" + std::to_string(shape.m) + " K" + std::to_string(shape.k) + " N" +
           std::to_string(shape.n);
}

} // namespace

std::size_t
run_gemm(fs::path const& xclbin_path, std::vector<gemm_case> const& shapes, options const& how)
{
    say("아티팩트 읽기");
    xrt::xclbin binary{xclbin_path.string()};
    std::printf("     %-40s %8ju B\n",
                xclbin_path.filename().string().c_str(),
                static_cast<std::uintmax_t>(fs::file_size(xclbin_path)));

    auto const design = operation::read_descriptor(binary);
    auto const fixed  = programs::gemm::describe(design);
    auto const stem   = xclbin_path.stem().string();

    say("설계 (xclbin USER_METADATA)");
    std::printf("     columns=%u rows=%u first_core_row=%u tile=%ux%ux%u element_bytes=%u\n",
                fixed.common.columns,
                fixed.rows,
                fixed.first_core_row,
                fixed.tile_m,
                fixed.tile_k,
                fixed.tile_n,
                fixed.element_bytes);
    std::printf("     runtime parameters");
    for (auto const slot : fixed.common.parameter_slots) std::printf(" 0x%05x", slot);
    std::printf(", start 0x%05x\n", fixed.common.start_register);
    std::printf("     인자 순서  a=%u  b=%u  c=%u\n",
                design.argument("a"),
                design.argument("b"),
                design.argument("c"));
    describe_signature(binary);

    // Every shape is wired first. A stream that is already wrong has nothing to learn from the
    // array, and a shape the design cannot run should not cost a device open to find out.
    struct wired
    {
        gemm_case                            shape;
        programs::gemm::parameters           param;
        std::unique_ptr<command_list>        stream;
    };

    std::vector<wired> ready{};

    for (auto const& shape : shapes)
    {
        std::printf("\n  --- %s\n", label_of(shape).c_str());

        programs::gemm::parameters param{
            .m = shape.m,
            .k = shape.k,
            .n = shape.n,
            .a = {.argument_index = design.argument("a")},
            .b = {.argument_index = design.argument("b")},
            .c = {.argument_index = design.argument("c")},
            .wait_for_inputs = how.drain,
        };

        programs::gemm const emitter{fixed, param};
        std::printf("     K 스텝 %u회, C 타일 %u개\n", emitter.k_steps(), emitter.tile_steps());

        auto stream =
            std::make_unique<command_list>(fixed.common.generation, fixed.common.partition_columns);

        if (auto const refused = emitter.wire(*stream))
        {
            std::printf("     wire 거부: %s\n", refused.message().c_str());
            continue;
        }
        if (not stream->finalize()) throw std::runtime_error("finalize failed");

        describe_header(stream->as_instructions());

        if (how.drain)
        {
            // A superset of what aiecc emits, so there is nothing to compare against. The numbers
            // are the check here.
            std::printf("     드레인 켬 -- aiecc 스트림과는 다르다 (의도된 것)\n");
        }
        else if (not shape.reference.empty() and
                 not matches_reference(stream->as_instructions(), shape.reference))
        {
            continue;
        }

        ready.push_back({shape, param, std::move(stream)});
    }

    if (how.dry)
    {
        std::printf("\n  건식: 여기까지. 디바이스는 열지 않는다\n");
        return ready.size();
    }

    // One array, every shape. That is the claim the design makes -- one xclbin serves every
    // projection because the cores read their trip counts out of their own memory -- so it is
    // tested by running them all against a single hardware context rather than opening one per
    // shape and never finding out.
    //
    // Under --isolate each shape gets a context of its own, which is what separates "this shape
    // cannot run" from "this shape cannot run after that one". The device stays the same either
    // way; it is the context that is in question. An operation is one context, so a second one
    // is a second load_op -- under a key of its own, since the registry keeps one per name.
    // Only for finding out: sixteen contexts is the driver's limit and a model has more shapes
    // than that, so nothing in the runtime may work this way.
    say("amd::device 열기");
    lnpu::nex::amd::device owner{0};

    auto const context_for = [&](std::string const& key) -> operation* {
        say("load_op (register_xclbin + hw_context)");
        if (auto const refused = owner.load_op(key, xrt::xclbin{binary}))
        {
            std::printf("     거부: %s\n", refused.message().c_str());
            return nullptr;
        }

        auto* const found = owner.op(key);
        if (found == nullptr) std::printf("     op() 가 null 을 돌려줌\n");
        return found;
    };

    operation* loaded = nullptr;

    if (not how.isolate)
    {
        loaded = context_for(stem);
        if (loaded == nullptr) return 0;
    }

    std::size_t passed = 0;

    for (auto& one : ready)
    {
        std::printf("\n  --- %s\n", label_of(one.shape).c_str());

        if (how.isolate)
        {
            loaded = context_for(stem + "." + label_of(one.shape));
            if (loaded == nullptr) continue;
        }

        say("피연산자와 CPU 레퍼런스 만들기");
        auto const started_reference = std::chrono::steady_clock::now();
        auto const made              = compute(one.shape);
        std::printf("     A %zu B, B %zu B, C %zu B  (레퍼런스 %.2f s)\n",
                    made.a.size() * 2,
                    made.b.size() * 2,
                    made.c.size() * 2,
                    std::chrono::duration<double>(std::chrono::steady_clock::now() -
                                                  started_reference)
                        .count());

        say("make<gemm> (operation 이 읽은 설계로)");
        auto const through_op = loaded->make<programs::gemm>(one.param);

        auto again =
            std::make_unique<command_list>(fixed.common.generation, fixed.common.partition_columns);
        if (auto const refused = through_op->wire(*again))
        {
            std::printf("     wire 거부: %s\n", refused.message().c_str());
            continue;
        }
        if (not again->finalize()) throw std::runtime_error("finalize failed");

        {
            auto const offline = one.stream->as_instructions();
            auto const online  = again->as_instructions();
            bool const same    = offline.size() == online.size() and
                              std::memcmp(offline.data(), online.data(), offline.size_bytes()) == 0;
            std::printf("     오프라인 스트림과 %s\n", same ? "바이트 일치" : "다름 (!)");
            if (not same) continue;
        }

        say("create_instance (aiebu -> elf -> module -> kernel)");
        auto running = loaded->create_instance(std::move(again));

        say("버퍼 (xrt::bo host_only)");
        auto a_bo = data_buffer(*owner.handle(), made.a.size() * 2);
        auto b_bo = data_buffer(*owner.handle(), made.b.size() * 2);
        auto c_bo = data_buffer(*owner.handle(), made.c.size() * 2);

        std::memcpy(a_bo.map(), made.a.data(), made.a.size() * 2);
        std::memcpy(b_bo.map(), made.b.data(), made.b.size() * 2);
        std::memset(c_bo.map(), 0, made.c.size() * 2);

        a_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
        b_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
        c_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);

        /**
         * One run, from a cleared C. Cleared on purpose: a stale result left over from the
         * previous run would read as a pass.
         */
        auto const run_once = [&](char const* what) -> deviation {
            std::memset(c_bo.map(), 0, made.c.size() * 2);
            c_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);

            auto const started = std::chrono::steady_clock::now();
            auto const refused = running->execute(a_bo, b_bo, c_bo);
            auto const spent =
                std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() -
                                                          started)
                    .count();

            if (refused)
            {
                std::printf("     %s 거부: %s\n", what, refused.message().c_str());
                return deviation{.relative_l2 = 1.0};
            }

            c_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

            auto const macs = 2.0 * one.shape.m * one.shape.k * one.shape.n;
            auto const got  = std::span{reinterpret_cast<std::uint16_t const*>(c_bo.map()),
                                        made.c.size()};
            auto const want = std::span{made.c.data(), made.c.size()};

            std::printf("     %s  %.1f us (%.1f GFLOP/s)\n", what, spent, macs / spent / 1e3);
            show_head(got, want);

            auto const off = measure(got, want, kRelativeTolerance, kAbsoluteTolerance);
            print(off, made.c.size());
            return off;
        };

        // More than once, because a run can be wrong in a way that depends on what ran before
        // it. Measured on hardware: after a shape with a different K, the first run computes as
        // though the previous K were still in force and the second does not recover. One
        // measurement cannot tell that from a design that simply cannot run the shape.
        say("execute");

        std::vector<deviation> runs{};
        for (int again = 0; again < how.verify; ++again)
        {
            runs.push_back(run_once((std::to_string(again + 1) + "회차").c_str()));
        }

        std::printf("     회차별 상대 L2 ");
        for (auto const& one : runs) std::printf(" %.4g", one.relative_l2);
        std::printf("\n");

        // Judged on the last one: whether the shape settles is what decides if it is usable, and
        // the run that led into it is reported above either way.
        bool const ok = runs.back().relative_l2 <= kRelativeL2;
        std::printf("     %s (마지막 회차 상대 L2 %.4g, 허용 %.4g)\n",
                    ok ? "통과" : "실패",
                    runs.back().relative_l2,
                    kRelativeL2);

        if (ok and runs.front().relative_l2 > kRelativeL2)
        {
            std::printf("     *** 처음에는 틀렸다가 자리를 잡았다\n");
        }

        if (ok) ++passed;

        if (how.repeats > 0)
        {
            auto const from = std::chrono::steady_clock::now();
            for (int at = 0; at < how.repeats; ++at) running->execute(a_bo, b_bo, c_bo);
            auto const spent =
                std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - from)
                    .count();
            auto const macs = 2.0 * one.shape.m * one.shape.k * one.shape.n;
            std::printf("     %d회 평균 %.1f us (%.1f GFLOP/s)\n",
                        how.repeats,
                        spent / how.repeats,
                        macs / (spent / how.repeats) / 1e3);
        }
    }

    return passed;
}

} // namespace hw
