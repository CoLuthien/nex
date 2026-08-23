
#include "cases.hpp"

#include "amd/descriptor.hpp"
#include "amd/device.hpp"
#include "amd/operation.hpp"
#include "amd/programs/rmsnorm.hpp"

#include <chrono>
#include <cstdio>
#include <memory>
#include <stdexcept>

namespace hw
{
namespace
{

using namespace lnpu::nex::amd;

/// bf16 rounding is what separates the array from a float reference; IRON allows the same.
constexpr float kRelativeTolerance = 0.04F;
constexpr float kAbsoluteTolerance = 1e-6F;

} // namespace

bool
run_rmsnorm(fs::path const& xclbin_path, fs::path const& reference, options const& how)
{
    say("아티팩트 읽기");
    xrt::xclbin binary{xclbin_path.string()};
    std::printf("     %-40s %8ju B\n",
                xclbin_path.filename().string().c_str(),
                static_cast<std::uintmax_t>(fs::file_size(xclbin_path)));

    // The same call the operation makes when it opens this xclbin, so the design this test wires
    // against and the one the runtime would read cannot be two different things.
    auto const design = operation::read_descriptor(binary);
    auto const fixed  = programs::rmsnorm::describe(design);
    auto const stem   = xclbin_path.stem();

    auto const input  = read_file(xclbin_path.parent_path() / (stem.string() + ".input.bin"));
    auto const golden = read_file(xclbin_path.parent_path() / (stem.string() + ".golden.bin"));
    auto const weight = fixed.weighted
                            ? read_file(xclbin_path.parent_path() / (stem.string() + ".weight.bin"))
                            : std::vector<char>{};

    auto const elements = static_cast<std::uint32_t>(input.size() / fixed.element_bytes);

    say("설계 (xclbin USER_METADATA)");
    std::printf("     columns=%u channels=%u tile=%u elements=%u weighted=%s element_bytes=%u\n",
                fixed.common.columns,
                fixed.channels,
                fixed.tile,
                fixed.elements,
                fixed.weighted ? "yes" : "no",
                fixed.element_bytes);

    // wire() refuses this too, but a size mismatch means the wrong artifact was picked up and
    // that is worth saying in those words rather than as a generic invalid_argument.
    if (elements != fixed.elements)
    {
        std::printf("     입력이 %u 원소인데 설계는 %u 원소용이다\n", elements, fixed.elements);
        return false;
    }

    // Bindings taken from the descriptor rather than counted by hand. The two rmsnorm designs
    // disagree about where output sits -- index 2 when there is a weight stream and 1 when there
    // is not -- and an index off by one reads a different buffer without complaint.
    std::printf("     인자 순서");
    for (auto const* which : {"input", "weight", "output"})
    {
        if (not fixed.weighted and std::string_view{which} == "weight") continue;
        std::printf("  %s=%u", which, design.argument(which));
    }
    std::printf("\n");
    describe_signature(binary);

    programs::rmsnorm::parameters param{};
    param.elements = elements;
    param.input    = {.argument_index = design.argument("input")};
    param.output   = {.argument_index = design.argument("output")};
    if (fixed.weighted) param.weight = {.argument_index = design.argument("weight")};

    say("명령 스트림 합성 (amd::programs::rmsnorm)");
    programs::rmsnorm const emitter{fixed, param};

    std::printf("     코어 %u개, 코어당 %u 원소, 컬럼당 BD %u개\n",
                emitter.cores(),
                emitter.slice(),
                emitter.buffer_descriptors_used());

    auto sequence =
        std::make_unique<command_list>(fixed.common.generation, fixed.common.partition_columns);
    if (auto const refused = emitter.wire(*sequence))
    {
        std::printf("     wire 거부: %s\n", refused.message().c_str());
        return false;
    }
    if (not sequence->finalize()) throw std::runtime_error("finalize failed");

    describe_header(sequence->as_instructions());

    // Compared before anything is sent anywhere: if the stream is already wrong, the hardware has
    // nothing to say about it.
    if (fs::exists(reference) and not matches_reference(sequence->as_instructions(), reference))
    {
        return false;
    }

    if (how.dry)
    {
        std::printf("  건식: 여기까지. 디바이스는 열지 않는다\n");
        return true;
    }

    say("amd::device 열기");
    lnpu::nex::amd::device owner{0};

    say("load_op (register_xclbin + hw_context)");
    if (auto const refused = owner.load_op(stem.string(), xrt::xclbin{binary}))
    {
        std::printf("     거부: %s\n", refused.message().c_str());
        return false;
    }

    auto* const loaded = owner.op(stem.string());
    if (loaded == nullptr)
    {
        std::printf("     op() 가 null 을 돌려줌\n");
        return false;
    }

    // The same program, this time built the way the runtime builds one: from the operation, which
    // read the design out of the xclbin it opened. The stream above came from a descriptor this
    // test parsed itself, so a difference here would mean the two paths disagree about the design
    // -- and only one of them is the one that runs.
    say("make<rmsnorm> (operation 이 읽은 설계로)");
    auto const through_op = loaded->make<programs::rmsnorm>(param);

    auto again =
        std::make_unique<command_list>(fixed.common.generation, fixed.common.partition_columns);
    if (auto const refused = through_op->wire(*again))
    {
        std::printf("     wire 거부: %s\n", refused.message().c_str());
        return false;
    }
    if (not again->finalize()) throw std::runtime_error("finalize failed");

    {
        auto const offline = sequence->as_instructions();
        auto const online  = again->as_instructions();
        bool const same    = offline.size() == online.size() and
                          std::memcmp(offline.data(), online.data(), offline.size_bytes()) == 0;
        std::printf("     오프라인 스트림과 %s\n", same ? "바이트 일치" : "다름 (!)");
        if (not same) return false;
    }

    say("create_instance (aiebu -> elf -> module -> kernel)");
    auto running = loaded->create_instance(std::move(again));

    say("버퍼 (xrt::bo host_only)");
    auto in_bo  = data_buffer(*owner.handle(), input.size());
    auto out_bo = data_buffer(*owner.handle(), golden.size());
    auto w_bo   = data_buffer(*owner.handle(), fixed.weighted ? weight.size() : std::size_t{4});

    std::memcpy(in_bo.map(), input.data(), input.size());
    std::memset(out_bo.map(), 0, golden.size());
    if (fixed.weighted) std::memcpy(w_bo.map(), weight.data(), weight.size());

    in_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    out_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    if (fixed.weighted) w_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    say("execute");
    auto const started = std::chrono::steady_clock::now();
    auto const refused =
        fixed.weighted ? running->execute(in_bo, w_bo, out_bo) : running->execute(in_bo, out_bo);
    auto const once =
        std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - started)
            .count();

    if (refused)
    {
        std::printf("     거부: %s\n", refused.message().c_str());
        return false;
    }
    std::printf("     완료, %.1f us (첫 실행)\n", once);

    out_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    auto const count = golden.size() / sizeof(std::uint16_t);
    auto const got   = std::span{reinterpret_cast<std::uint16_t const*>(out_bo.map()), count};
    auto const want  = std::span{reinterpret_cast<std::uint16_t const*>(golden.data()), count};

    say("결과");
    show_head(got, want);

    // Every output of a normalization is the size of its input, so per-element relative error is
    // the right measure here and every element has to pass it.
    auto const off = measure(got, want, kRelativeTolerance, kAbsoluteTolerance);
    print(off, count);
    bool const ok = off.outside == 0;

    if (how.repeats > 0)
    {
        auto const from = std::chrono::steady_clock::now();
        for (int at = 0; at < how.repeats; ++at)
        {
            if (fixed.weighted)
                running->execute(in_bo, w_bo, out_bo);
            else
                running->execute(in_bo, out_bo);
        }
        auto const spent =
            std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - from)
                .count();
        std::printf("     %d회 평균 %.1f us\n", how.repeats, spent / how.repeats);
    }

    return ok;
}

} // namespace hw
