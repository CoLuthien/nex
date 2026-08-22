// Runs an RMSNorm through this repository's own backend on a real NPU and checks the numbers.
//
// Nothing here talks to XRT for the run itself. The instruction stream is built by
// amd::programs::rmsnorm, the array is opened by amd::device, and the run goes through
// amd::operation -- the same objects the runtime is made of. Going around them, as an earlier
// version of this did, hides exactly the kind of gap that matters: device had no constructor at
// all and nobody noticed, because nothing had ever constructed one.
//
// Everything the backend needs is in headers with no XRT of their own, so this builds against
// XRT and spdlog alone -- no protobuf, no onnx, none of the rest of the tree.

#include "amd/descriptor.hpp"
#include "amd/device.hpp"
#include "amd/programs/rmsnorm.hpp"

#include <xrt/xrt_bo.h>
#include <xrt/xrt_device.h>
#include <xrt/experimental/xrt_xclbin.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;
using namespace lnpu::nex::amd;

namespace
{

// ---------------------------------------------------------------------------------------------

void
say(char const* what)
{
    std::printf("  .. %s\n", what);
    std::fflush(stdout);
}

float
from_bf16(std::uint16_t raw)
{
    std::uint32_t const bits = static_cast<std::uint32_t>(raw) << 16;
    float               out{};
    std::memcpy(&out, &bits, sizeof(out));
    return out;
}

std::vector<char>
read_file(fs::path const& path)
{
    std::ifstream in{path, std::ios::binary};
    if (not in) throw std::runtime_error("cannot read " + path.string());

    std::vector<char> bytes{std::istreambuf_iterator<char>{in}, std::istreambuf_iterator<char>{}};

    std::printf("     %-28s %8zu B\n", path.filename().string().c_str(), bytes.size());
    return bytes;
}

// ---------------------------------------------------------------------------------------------
// The design descriptor the baking script wrote into the xclbin, so nothing here restates it.

std::string_view
user_metadata(xrt::xclbin const& binary)
{
    auto const* top = binary.get_axlf();
    if (top == nullptr) return {};

    for (std::uint32_t at = 0; at < top->m_header.m_numSections; ++at)
    {
        auto const& header = top->m_sections[at];
        if (header.m_sectionKind != static_cast<std::uint32_t>(USER_METADATA)) continue;

        return {reinterpret_cast<char const*>(top) + header.m_sectionOffset,
                static_cast<std::size_t>(header.m_sectionSize)};
    }
    return {};
}

void
describe_signature(xrt::xclbin const& binary)
{
    for (auto const& one : binary.get_kernels())
    {
        std::printf("     커널 %s, 인자 %zu개\n", one.get_name().c_str(), one.get_num_args());
        for (auto const& arg : one.get_args())
        {
            std::printf("       [%zu] %-7s size=%zu offset=%zu %s\n",
                        static_cast<std::size_t>(arg.get_index()),
                        arg.get_name().c_str(),
                        arg.get_size(),
                        arg.get_offset(),
                        arg.get_host_type().c_str());
        }
    }
}

/// The four words every transaction stream opens with, so a malformed one is visible here rather
/// than as a hang on the device.
void
describe_header(std::span<command::word const> stream)
{
    if (stream.size() < 4)
    {
        std::printf("     스트림이 헤더보다 짧다 (%zu 워드)\n", stream.size());
        return;
    }

    std::printf("     헤더 major=%u minor=%u generation=%u rows=%u cols=%u memtile=%u\n",
                stream[0] & 0xFFU,
                (stream[0] >> 8) & 0xFFU,
                (stream[0] >> 16) & 0xFFU,
                (stream[0] >> 24) & 0xFFU,
                stream[1] & 0xFFU,
                (stream[1] >> 8) & 0xFFU);
    std::printf("     명령 %u개, %u B (파일 %zu B)\n",
                stream[2],
                stream[3],
                stream.size() * sizeof(command::word));
}

// ---------------------------------------------------------------------------------------------

struct deviation
{
    std::size_t worst_at{};
    float       worst_rel{};
    float       worst_abs{};
    std::size_t outside{};
    std::size_t zeros{};
};

deviation
compare(std::span<std::uint16_t const> got,
        std::span<std::uint16_t const> want,
        float                          rel_tol,
        float                          abs_tol)
{
    deviation found{};
    for (std::size_t at = 0; at < want.size(); ++at)
    {
        auto const a   = from_bf16(got[at]);
        auto const b   = from_bf16(want[at]);
        auto const abs = std::fabs(a - b);
        auto const rel = std::fabs(b) > 0.0F ? abs / std::fabs(b) : abs;

        if (a == 0.0F) ++found.zeros;
        if (rel > found.worst_rel)
        {
            found.worst_rel = rel;
            found.worst_abs = abs;
            found.worst_at  = at;
        }
        if (abs > abs_tol and rel > rel_tol) ++found.outside;
    }
    return found;
}

// ---------------------------------------------------------------------------------------------

/// Allocated the one way that works on both driver stacks. An xrt::bo subclass handed to the
/// variadic kernel call is taken for a scalar by older XRT and rejected as
/// "patch_value() only supports 64-bit values or less".
xrt::bo
data_buffer(xrt::device const& device, std::size_t bytes)
{
    return xrt::bo{device, bytes, xrt::bo::flags::host_only, xrt::memory_group{0}};
}

bool
run_case(fs::path const& dir, std::string const& name, int repeats, bool dry)
{
    std::printf("\n=== %s\n", name.c_str());

    say("아티팩트 읽기");
    auto const  xclbin_path = dir / (name + ".xclbin");
    xrt::xclbin binary{xclbin_path.string()};
    std::printf("     %-28s %8ju B\n",
                xclbin_path.filename().string().c_str(),
                static_cast<std::uintmax_t>(fs::file_size(xclbin_path)));

    auto const metadata = user_metadata(binary);
    if (metadata.empty()) throw std::runtime_error("xclbin carries no USER_METADATA");

    auto const design = descriptor::parse(metadata);
    if (design.op() != "rmsnorm")
        throw std::runtime_error("this test runs rmsnorm, not '" + std::string{design.op()} + "'");

    auto const common   = design.common();
    auto const channels = design.u32("channels");
    auto const bytes    = design.u32("element_bytes");
    auto const weighted = design.flag("weighted");

    auto const input  = read_file(dir / (name + ".input.bin"));
    auto const golden = read_file(dir / (name + ".golden.bin"));
    auto const weight = weighted ? read_file(dir / (name + ".weight.bin")) : std::vector<char>{};

    auto const elements = static_cast<std::uint32_t>(input.size() / bytes);

    say("설계 (xclbin USER_METADATA)");
    std::printf("     columns=%u channels=%u tile=%u elements=%u weighted=%s element_bytes=%u\n",
                common.columns,
                channels,
                design.u32("tile"),
                design.u32("elements"),
                weighted ? "yes" : "no",
                bytes);

    // The design fixes the size: its cores loop a count that is a compile-time constant in the
    // IRON design, not a runtime parameter, so a design runs exactly one length. Feeding it a
    // longer input does not run longer -- it runs the first slice and leaves the rest untouched,
    // which looks like a partly-correct answer rather than a failure.
    if (elements != design.u32("elements"))
    {
        std::printf("     입력이 %u 원소인데 설계는 %u 원소용이다\n",
                    elements,
                    design.u32("elements"));
        return false;
    }

    std::printf("     인자 순서");
    for (auto const* which : {"input", "weight", "output"})
    {
        if (not weighted and std::string_view{which} == "weight") continue;
        std::printf("  %s=%u", which, design.argument(which));
    }
    std::printf("\n");

    describe_signature(binary);

    // ---- the instruction stream, built here by the program the runtime uses
    say("명령 스트림 합성 (amd::programs::rmsnorm)");

    programs::rmsnorm::parameters param{};
    param.design        = common;
    param.channels      = channels;
    param.elements      = elements;
    param.element_bytes = bytes;
    param.weighted      = weighted;

    // Taken from the descriptor rather than counted by hand. The two rmsnorm designs disagree
    // about where output sits -- index 2 when there is a weight stream and 1 when there is not --
    // and an index off by one reads a different buffer without complaint.
    param.input  = {.argument_index = design.argument("input")};
    param.output = {.argument_index = design.argument("output")};
    if (weighted) param.weight = {.argument_index = design.argument("weight")};

    programs::rmsnorm const emitter{param};

    std::printf("     코어 %u개, 코어당 %u 원소, 컬럼당 BD %u개\n",
                emitter.cores(),
                emitter.slice(),
                emitter.buffer_descriptors_used());

    auto sequence = std::make_unique<command_list>(common.generation, common.partition_columns);
    if (auto const refused = emitter.wire(*sequence))
    {
        std::printf("     wire 거부: %s\n", refused.message().c_str());
        return false;
    }
    if (not sequence->finalize()) throw std::runtime_error("finalize failed");

    describe_header(sequence->as_instructions());

    // Compared before anything is sent anywhere: if the stream is already wrong, the hardware has
    // nothing to say about it.
    if (auto const reference = dir / (name + ".aiecc.bin"); fs::exists(reference))
    {
        auto const theirs = read_file(reference);
        auto const ours   = sequence->as_instructions();
        bool const same   = theirs.size() == ours.size() * sizeof(command::word) and
                            std::memcmp(theirs.data(), ours.data(), theirs.size()) == 0;
        std::printf("     aiecc 스트림과 %s\n", same ? "바이트 일치" : "다름 (!)");
        if (not same) return false;
    }

    if (dry)
    {
        std::printf("  건식: 여기까지. 디바이스는 열지 않는다\n");
        return true;
    }

    // ---- the backend
    say("amd::device 열기");
    lnpu::nex::amd::device owner{0};

    say("load_op (register_xclbin + hw_context)");
    if (auto const refused = owner.load_op(name, xrt::xclbin{binary}))
    {
        std::printf("     거부: %s\n", refused.message().c_str());
        return false;
    }

    auto* const loaded = owner.op(name);
    if (loaded == nullptr)
    {
        std::printf("     op() 가 null 을 돌려줌\n");
        return false;
    }

    say("create_instance (aiebu -> elf -> module -> kernel)");
    auto running = loaded->create_instance(std::move(sequence));

    // ---- buffers
    say("버퍼 (xrt::bo host_only)");
    auto in_bo  = data_buffer(*owner.handle(), input.size());
    auto out_bo = data_buffer(*owner.handle(), golden.size());
    auto w_bo   = data_buffer(*owner.handle(), weighted ? weight.size() : std::size_t{4});

    std::printf("     input %zu B @ %#llx, output %zu B @ %#llx",
                input.size(),
                static_cast<unsigned long long>(in_bo.address()),
                golden.size(),
                static_cast<unsigned long long>(out_bo.address()));
    if (weighted)
        std::printf(", weight %zu B @ %#llx",
                    weight.size(),
                    static_cast<unsigned long long>(w_bo.address()));
    std::printf("\n");

    std::memcpy(in_bo.map(), input.data(), input.size());
    std::memset(out_bo.map(), 0, golden.size());
    if (weighted) std::memcpy(w_bo.map(), weight.data(), weight.size());

    in_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    out_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    if (weighted) w_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    // ---- run
    say("execute");
    auto const started = std::chrono::steady_clock::now();
    auto const refused =
        weighted ? running->execute(in_bo, w_bo, out_bo) : running->execute(in_bo, out_bo);
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

    // ---- numbers
    auto const got  = std::span{reinterpret_cast<std::uint16_t const*>(out_bo.map()),
                                golden.size() / sizeof(std::uint16_t)};
    auto const want = std::span{reinterpret_cast<std::uint16_t const*>(golden.data()),
                                golden.size() / sizeof(std::uint16_t)};

    say("결과");
    std::printf("     나온 값");
    for (std::size_t at = 0; at < 6 and at < got.size(); ++at)
        std::printf(" %8.4f", static_cast<double>(from_bf16(got[at])));
    std::printf("\n     정답   ");
    for (std::size_t at = 0; at < 6 and at < want.size(); ++at)
        std::printf(" %8.4f", static_cast<double>(from_bf16(want[at])));
    std::printf("\n");

    auto const off = compare(got, want, 0.04F, 1e-6F);
    std::printf("     최대 상대오차 %.4g (원소 %zu, 절대 %.4g)\n",
                static_cast<double>(off.worst_rel),
                off.worst_at,
                static_cast<double>(off.worst_abs));
    std::printf("     허용 밖 %zu / %zu, 0 인 원소 %zu개\n", off.outside, want.size(), off.zeros);

    if (off.zeros == want.size())
    {
        std::printf("     (전부 0 -- 커널이 이 버퍼에 쓰지 않았다는 뜻이다)\n");
    }

    if (repeats > 0)
    {
        auto const from = std::chrono::steady_clock::now();
        for (int at = 0; at < repeats; ++at)
        {
            if (weighted)
                running->execute(in_bo, w_bo, out_bo);
            else
                running->execute(in_bo, out_bo);
        }
        auto const spent =
            std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - from)
                .count();
        std::printf("     %d회 평균 %.1f us\n", repeats, spent / repeats);
    }

    bool const ok = off.outside == 0;
    std::printf("  %s\n", ok ? "통과" : "실패");
    return ok;
}

} // namespace

int
main(int argc, char** argv)
{
    fs::path const dir     = argc > 1 ? argv[1] : "artifacts";
    int const      repeats = argc > 2 ? std::atoi(argv[2]) : 100;
    bool const     dry     = argc > 3 and std::string_view{argv[3]} == "--dry";

    std::size_t ran = 0, passed = 0;

    try
    {
        auto const       index = read_file(dir / "index.json");
        std::string_view text{index.data(), index.size()};

        for (std::size_t at = text.find("\"name\""); at != std::string_view::npos;
             at             = text.find("\"name\"", at + 1))
        {
            auto const        open  = text.find('"', text.find(':', at)) + 1;
            auto const        close = text.find('"', open);
            std::string const name{text.substr(open, close - open)};

            ++ran;
            try
            {
                if (run_case(dir, name, repeats, dry)) ++passed;
            }
            catch (std::exception const& failure)
            {
                std::printf("  예외: %s\n", failure.what());
            }
        }
    }
    catch (std::exception const& failure)
    {
        std::printf("실패: %s\n", failure.what());
        return 2;
    }

    std::printf("\n%zu개 중 %zu개 통과\n", ran, passed);
    return passed == ran ? 0 : 1;
}
