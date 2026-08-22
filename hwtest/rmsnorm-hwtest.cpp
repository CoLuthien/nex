// Runs an RMSNorm instruction stream this repository emits on a real NPU and checks the numbers.
//
// The stream is not read from a file: it is built here by amd::programs::rmsnorm, the same code
// the runtime uses. On a machine with no NPU that emitter has only ever been checked against the
// streams aiecc produced, which proves the bytes agree but not that the hardware accepts them or
// that what comes back is right. That is what this is for.
//
// Everything the emitter needs is in a handful of headers with no XRT in them, so this builds
// against XRT alone -- no protobuf, no onnx, none of the rest of the tree.

#include "amd/programs/rmsnorm.hpp"

#include <xrt/xrt_bo.h>
#include <xrt/xrt_device.h>
#include <xrt/xrt_kernel.h>

#include <xrt/experimental/xrt_elf.h>
#include <xrt/experimental/xrt_ext.h>
#include <xrt/experimental/xrt_module.h>
#include <xrt/experimental/xrt_xclbin.h>

#include <aiebu/aiebu.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <functional>
#include <fstream>
#include <stdexcept>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;
using namespace lnpu::nex::amd;

namespace
{

// ---------------------------------------------------------------------------------------------
// bf16, only as much of it as comparing two buffers needs

float
from_bf16(std::uint16_t raw)
{
    std::uint32_t const bits = static_cast<std::uint32_t>(raw) << 16;
    float               out{};
    std::memcpy(&out, &bits, sizeof(out));
    return out;
}

// ---------------------------------------------------------------------------------------------

std::vector<char>
read_file(fs::path const& path)
{
    std::ifstream in{path, std::ios::binary};
    if (not in) throw std::runtime_error("cannot read " + path.string());
    return {std::istreambuf_iterator<char>{in}, std::istreambuf_iterator<char>{}};
}

/// The design descriptor the baking script wrote into the xclbin, so nothing here restates it.
std::string_view
user_metadata(xrt::xclbin const& binary)
{
    auto const* top = binary.get_axlf();
    if (top == nullptr) return {};

    for (int at = 0; at < top->m_header.m_numSections; ++at)
    {
        auto const& header = top->m_sections[at];
        if (header.m_sectionKind != static_cast<std::uint32_t>(USER_METADATA)) continue;

        return {reinterpret_cast<char const*>(top) + header.m_sectionOffset,
                static_cast<std::size_t>(header.m_sectionSize)};
    }
    return {};
}

/// Enough of a json reader for flat "key": value pairs; the descriptor has nothing nested.
long
number_in(std::string_view json, std::string_view key)
{
    auto const at = json.find("\"" + std::string{key} + "\"");
    if (at == std::string_view::npos) throw std::runtime_error("no '" + std::string{key} + "'");
    auto const colon = json.find(':', at);
    return std::strtol(json.data() + colon + 1, nullptr, 10);
}

bool
flag_in(std::string_view json, std::string_view key)
{
    auto const at = json.find("\"" + std::string{key} + "\"");
    if (at == std::string_view::npos) throw std::runtime_error("no '" + std::string{key} + "'");
    return json.find("true", at) < json.find(',', at);
}

/// mlir-aie names its one kernel this; the instance suffix varies, so the prefix is matched.
std::string
kernel_named(xrt::xclbin const& binary)
{
    for (auto const& kernel : binary.get_kernels())
    {
        if (kernel.get_name().rfind("MLIR_AIE", 0) == 0) return kernel.get_name();
    }
    throw std::runtime_error("xclbin declares no MLIR_AIE kernel");
}

std::vector<char>
assemble(std::span<command::word const> stream)
{
    void*    elf{nullptr};
    unsigned size = aiebu_assembler_get_elf(aiebu_assembler_buffer_type_blob_instr_transaction,
                                            reinterpret_cast<char const*>(stream.data()),
                                            stream.size() * sizeof(command::word),
                                            nullptr,
                                            0,
                                            &elf,
                                            nullptr,
                                            0,
                                            "",
                                            "",
                                            nullptr,
                                            0);
    if (elf == nullptr or size == 0) throw std::runtime_error("aiebu produced no elf");

    std::vector<char> out(static_cast<char*>(elf), static_cast<char*>(elf) + size);
    std::free(elf);
    return out;
}

struct deviation
{
    std::size_t worst_at{};
    float       worst_rel{};
    float       worst_abs{};
    std::size_t outside{};
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

/// Everything a case needs, and everything building it produced.
struct case_inputs
{
    xrt::xclbin       binary;
    command_list      sequence{npu::npu2, 8};
    std::vector<char> input, weight, golden;
    bool              weighted{};
};

/// The half that needs no device: build the stream and check it against what aiecc produced.
/// Splitting it out means a machine with no NPU can still tell whether the artifacts and the
/// emitter agree, which is the first thing to know when the hardware run misbehaves.
bool
build_case(fs::path const& dir, std::string const& name, case_inputs& built)
{
    std::printf("\n=== %s\n", name.c_str());

    built.binary = xrt::xclbin{(dir / (name + ".xclbin")).string()};

    auto const metadata = user_metadata(built.binary);
    if (metadata.empty()) throw std::runtime_error("xclbin carries no USER_METADATA");

    auto const columns  = static_cast<std::uint32_t>(number_in(metadata, "columns"));
    auto const channels = static_cast<std::uint32_t>(number_in(metadata, "channels"));
    auto const bytes    = static_cast<std::uint32_t>(number_in(metadata, "element_bytes"));
    auto const weighted = flag_in(metadata, "weighted");

    built.weighted = weighted;
    built.input    = read_file(dir / (name + ".input.bin"));
    built.golden   = read_file(dir / (name + ".golden.bin"));
    built.weight   = weighted ? read_file(dir / (name + ".weight.bin")) : std::vector<char>{};

    auto const elements = static_cast<std::uint32_t>(built.input.size() / bytes);

    std::printf("  설계: columns=%u channels=%u weighted=%s, 원소 %u개\n",
                columns,
                channels,
                weighted ? "yes" : "no",
                elements);

    // ---- 명령 스트림을 여기서 만든다 (파일에서 읽지 않는다)
    programs::rmsnorm::parameters param{};
    param.design.op                = "rmsnorm";
    param.design.generation        = npu::npu2;
    param.design.partition_columns = 8;
    param.design.columns           = columns;
    param.channels                 = channels;
    param.elements                 = elements;
    param.element_bytes            = bytes;
    param.weighted                 = weighted;
    param.input                    = {.argument_index = 0};
    param.weight                   = {.argument_index = 1};
    param.output                   = {.argument_index = weighted ? 2U : 1U};

    programs::rmsnorm const emitter{std::move(param)};

    if (auto const failure = emitter.wire(built.sequence))
    {
        std::printf("  wire 실패: %s\n", failure.message().c_str());
        return false;
    }
    if (not built.sequence.finalize()) throw std::runtime_error("finalize failed");

    auto const stream = built.sequence.as_instructions();
    std::printf("  명령 스트림: %zu 워드 (%zu 바이트)\n",
                stream.size(),
                stream.size() * sizeof(command::word));

    // 참고: aiecc 가 만든 것과 같은지 (있을 때만)
    if (auto const reference = dir / (name + ".aiecc.bin"); fs::exists(reference))
    {
        auto const theirs = read_file(reference);
        auto const same   = theirs.size() == stream.size() * sizeof(command::word) and
                            std::memcmp(theirs.data(), stream.data(), theirs.size()) == 0;
        std::printf("  aiecc 스트림과 %s\n", same ? "바이트 일치" : "다름 (!)");
        if (not same) return false;
    }

    return true;
}

/// Names each step so a failure says which call threw rather than only what it said.
template <typename F>
auto
step(char const* what, F&& body) -> decltype(body())
{
    std::printf("  .. %s\n", what);
    std::fflush(stdout);
    return body();
}

void
describe_signature(xrt::xclbin const& binary, std::string const& kernel)
{
    for (auto const& one : binary.get_kernels())
    {
        if (one.get_name() != kernel) continue;

        std::printf("  커널 인자 %zu개:\n", one.get_num_args());
        for (auto const& arg : one.get_args())
        {
            std::printf("    [%d] %-8s size=%zu offset=%zu host_type=%s\n",
                        arg.get_index(),
                        arg.get_name().c_str(),
                        arg.get_size(),
                        arg.get_offset(),
                        arg.get_host_type().c_str());
        }
    }
}

/// The half that needs the NPU.
bool
run_case(xrt::device& device, case_inputs const& built, int repeats)
{
    auto const  stream   = built.sequence.as_instructions();
    auto const  weighted = built.weighted;
    auto const& binary   = built.binary;
    auto const& input    = built.input;
    auto const& weight   = built.weight;
    auto const& golden   = built.golden;

    auto const name = kernel_named(binary);
    describe_signature(binary, name);

    auto const uuid    = step("register_xclbin", [&] { return device.register_xclbin(binary); });
    auto       context = step("hw_context", [&] { return xrt::hw_context{device, uuid}; });

    auto const elf_bytes = step("aiebu -> elf", [&] { return assemble(stream); });
    auto       executable =
        step("xrt::elf", [&] { return xrt::elf{elf_bytes.data(), elf_bytes.size()}; });
    auto module = step("xrt::module", [&] { return xrt::module{executable}; });
    auto kernel = step("xrt::ext::kernel", [&] { return xrt::ext::kernel{context, module, name}; });

    // Two ways to get the instructions to the device, and the xclbin does not say which it wants.
    //
    //   elf     -- aiebu wraps the transaction blob in an ELF and XRT patches the buffer addresses
    //              into it. This is what operation.cpp does, and what FastFlowLM does with its own
    //              xclbins.
    //   legacy  -- the words go into a plain cacheable buffer and ride as arguments 1 and 2.
    //              This is what mlir-aie's own runtime does with an aiecc .bin, so it is the flow
    //              these xclbins were built against. XRT calls it deprecated but still takes it.
    auto plain = step("xrt::kernel (legacy)", [&] { return xrt::kernel{context, name}; });

    auto const stream_bytes = stream.size() * sizeof(command::word);
    auto       insts_bo     = step("bo(insts, cacheable)", [&] {
        return xrt::bo{device, stream_bytes, xrt::bo::flags::cacheable, plain.group_id(1)};
    });
    std::memcpy(insts_bo.map(), stream.data(), stream_bytes);
    insts_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    auto in_bo  = step("bo(input)", [&] { return xrt::ext::bo{device, input.size()}; });
    auto out_bo = step("bo(output)", [&] { return xrt::ext::bo{device, golden.size()}; });

    std::memcpy(in_bo.map(), input.data(), input.size());
    std::memset(out_bo.map(), 0, golden.size());
    in_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    out_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    // A weightless design has no third buffer; xrt::ext::bo has no empty state, so the slot is
    // filled with a one-byte buffer that is never handed to the kernel.
    auto weight_bo = step("bo(weight)", [&] {
        return xrt::ext::bo{device, weighted ? weight.size() : std::size_t{4}};
    });
    if (weighted)
    {
        std::memcpy(weight_bo.map(), weight.data(), weight.size());
        weight_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    }

    // Two ways to hand over the arguments, because it is not settled which one an ELF-flow kernel
    // wants. The three leading scalars are what the xclbin's signature asks for and what
    // FastFlowLM passes, but the module already carries the instructions, so they may be the
    // driver's business rather than ours. Whichever one runs is the answer.
    constexpr std::uint64_t kStartNpu = 3;

    auto with_scalars = [&] {
        return weighted ? kernel(kStartNpu, 0, 0, in_bo, weight_bo, out_bo)
                        : kernel(kStartNpu, 0, 0, in_bo, out_bo);
    };
    auto buffers_only = [&] {
        return weighted ? kernel(in_bo, weight_bo, out_bo) : kernel(in_bo, out_bo);
    };
    // mlir-aie passes the buffer's size in bytes here, not the word count.
    auto legacy = [&] {
        return weighted ? plain(kStartNpu, insts_bo, stream_bytes, in_bo, weight_bo, out_bo)
                        : plain(kStartNpu, insts_bo, stream_bytes, in_bo, out_bo);
    };

    ert_cmd_state state{};
    bool          ran = false;
    char const*   how = "";

    for (auto const& [label, launch] :
         {std::pair<char const*, std::function<xrt::run()>>{"elf (3,0,0,bo..)", with_scalars},
          std::pair<char const*, std::function<xrt::run()>>{"elf (bo.. 만)", buffers_only},
          std::pair<char const*, std::function<xrt::run()>>{"legacy (3,insts_bo,n,bo..)", legacy}})
    {
        try
        {
            std::printf("  .. 실행 %s\n", label);
            std::fflush(stdout);
            auto run = launch();
            state    = run.wait();
            ran      = true;
            how      = label;
            break;
        }
        catch (std::exception const& failure)
        {
            std::printf("     거부: %s\n", failure.what());
        }
    }

    if (not ran)
    {
        std::printf("  두 방식 모두 실패\n");
        return false;
    }

    std::printf("  실행됨 %s, 상태 %d (4 = COMPLETED)\n", how, static_cast<int>(state));

    out_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    auto const got  = std::span{reinterpret_cast<std::uint16_t const*>(out_bo.map()),
                                golden.size() / sizeof(std::uint16_t)};
    auto const want = std::span{reinterpret_cast<std::uint16_t const*>(golden.data()),
                                golden.size() / sizeof(std::uint16_t)};

    auto const off = compare(got, want, 0.04F, 1e-6F);
    std::printf("  최대 상대오차 %.4g (원소 %zu, 절대 %.4g), 허용 밖 %zu / %zu\n",
                static_cast<double>(off.worst_rel),
                off.worst_at,
                static_cast<double>(off.worst_abs),
                off.outside,
                want.size());

    if (repeats > 0)
    {
        auto const started = std::chrono::steady_clock::now();
        for (int at = 0; at < repeats; ++at)
        {
            (std::string_view{how}.starts_with("legacy")                     ? legacy()
             : std::string_view{how}.find("3,0,0") != std::string_view::npos ? with_scalars()
                                                                             : buffers_only())
                .wait();
        }
        auto const spent =
            std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - started)
                .count();
        std::printf("  %d회 평균 %.1f us\n", repeats, spent / repeats);
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

    try
    {
        std::unique_ptr<xrt::device> device{};
        if (dry)
        {
            std::printf("건식 실행: 스트림 생성까지만 확인하고 디바이스는 열지 않는다\n");
        }
        else
        {
            device = std::make_unique<xrt::device>(0);
            std::printf("디바이스: %s\n", device->get_info<xrt::info::device::name>().c_str());
        }

        // index.json 의 name 목록만 훑는다.
        auto const       index = read_file(dir / "index.json");
        std::string_view text{index.data(), index.size()};

        std::size_t passed = 0, ran = 0;
        for (std::size_t at = text.find("\"name\""); at != std::string_view::npos;
             at             = text.find("\"name\"", at + 1))
        {
            auto const        open  = text.find('"', text.find(':', at)) + 1;
            auto const        close = text.find('"', open);
            std::string const name{text.substr(open, close - open)};

            ++ran;
            try
            {
                case_inputs built{};
                if (not build_case(dir, name, built)) continue;
                if (dry)
                {
                    ++passed;
                    continue;
                }
                if (run_case(*device, built, repeats)) ++passed;
            }
            catch (std::exception const& failure)
            {
                std::printf("  예외: %s\n", failure.what());
            }
        }

        std::printf("\n%zu개 중 %zu개 통과\n", ran, passed);
        return passed == ran ? 0 : 1;
    }
    catch (std::exception const& failure)
    {
        std::printf("실패: %s\n", failure.what());
        return 2;
    }
}
