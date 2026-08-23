
// Runs this repository's own emitter against every baked artifact, on a real NPU.
//
// Nothing here talks to XRT for the run itself. The instruction stream is built by an
// amd::program, the array is opened by amd::device, and the run goes through amd::operation --
// the same objects the runtime is made of. Going around them, as an earlier version of this did,
// hides exactly the kind of gap that matters: device had no constructor at all and nobody
// noticed, because nothing had ever constructed one.
//
// Cases are discovered, not listed. Every xclbin in the directory is opened, its descriptor says
// which operator it is, and the aiecc streams sitting beside it say which shapes to run: one
// per stream. So baking a design adds its cases, and nothing has to be kept in step by hand.
//
// Everything the backend needs is in headers with no protobuf or onnx behind them, so this builds
// against XRT and spdlog alone.

#include "cases.hpp"

#include "amd/descriptor.hpp"
#include "amd/operation.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <map>
#include <string>
#include <string_view>

namespace
{
namespace fs = std::filesystem;
using namespace lnpu::nex::amd;

/**
 * @brief Reads "...m128k3072n1536.aiecc.bin" back into a shape.
 *
 * A shape-agnostic design is one array and a stream per shape, so the shape has to be written
 * somewhere. It is written in the file name -- bake.py puts it there -- rather than in a sidecar,
 * because a name cannot go missing and cannot disagree with the file it names.
 */
bool
shape_in(std::string const& name, hw::gemm_case& into)
{
    auto const at = name.find(".m");
    if (at == std::string::npos) return false;

    return std::sscanf(name.c_str() + at, ".m%uk%un%u", &into.m, &into.k, &into.n) == 3;
}

/// Streams beside an xclbin: "<stem>.aiecc.bin" and "<stem>.<shape>.aiecc.bin".
std::vector<fs::path>
references_for(fs::path const& xclbin)
{
    auto const           stem = xclbin.stem().string();
    std::vector<fs::path> found{};

    for (auto const& entry : fs::directory_iterator{xclbin.parent_path()})
    {
        auto const name = entry.path().filename().string();
        if (name.starts_with(stem) and name.ends_with(".aiecc.bin"))
        {
            found.push_back(entry.path());
        }
    }

    std::sort(found.begin(), found.end());
    return found;
}

struct tally
{
    std::size_t ran{};
    std::size_t passed{};
};

void
run_one(fs::path const& xclbin, hw::options const& how, tally& count)
{
    std::string op{};
    try
    {
        xrt::xclbin binary{xclbin.string()};
        op = operation::read_descriptor(binary).op();
    }
    catch (std::exception const& failure)
    {
        std::printf("\n=== %s\n  설계를 읽을 수 없다: %s\n",
                    xclbin.filename().string().c_str(),
                    failure.what());
        ++count.ran;
        return;
    }

    auto const references = references_for(xclbin);

    if (op == "rmsnorm")
    {
        // One design, one length: the count is baked into the cores, so there is one case here
        // however many streams happen to sit beside it.
        auto const stem = xclbin.stem().string();

        // An explicit order names what to run, so a design it does not name is not run at all.
        auto const asked = how.order.empty()
                               ? (how.only.empty() or stem.find(how.only) != std::string::npos
                                      ? std::size_t{1}
                                      : std::size_t{0})
                               : static_cast<std::size_t>(std::count_if(
                                     how.order.begin(), how.order.end(), [&](auto const& one) {
                                         return stem.find(one) != std::string::npos;
                                     }));

        for (std::size_t again = 0; again < asked; ++again)
        {
            std::printf("\n=== %s\n", stem.c_str());
            ++count.ran;
            if (hw::run_rmsnorm(xclbin, references.empty() ? fs::path{} : references.front(), how))
            {
                ++count.passed;
            }
        }
        return;
    }

    if (op == "gemm")
    {
        // Without an order, every stream beside the xclbin, once each, in name order. With one,
        // exactly what it names, in that order -- a name may appear twice, which is how a shape
        // is run again after another one has been through the same context.
        std::vector<fs::path> wanted{};

        if (how.order.empty())
        {
            for (auto const& reference : references)
            {
                if (how.only.empty() or
                    reference.filename().string().find(how.only) != std::string::npos)
                {
                    wanted.push_back(reference);
                }
            }
        }
        else
        {
            for (auto const& one : how.order)
            {
                auto const found =
                    std::find_if(references.begin(), references.end(), [&](auto const& reference) {
                        return reference.filename().string().find(one) != std::string::npos;
                    });

                if (found != references.end()) wanted.push_back(*found);
            }
        }

        std::vector<hw::gemm_case> shapes{};
        for (auto const& reference : wanted)
        {
            auto const named = reference.filename().string();

            hw::gemm_case shape{};
            if (not shape_in(named, shape))
            {
                std::printf("  %s 는 shape 를 이름에 담고 있지 않아 건너뛴다\n", named.c_str());
                continue;
            }
            shape.reference = reference;
            shapes.push_back(shape);
        }

        if (shapes.empty())
        {
            if (how.only.empty() and how.order.empty())
            {
                std::printf("\n=== %s\n  돌릴 shape 가 없다\n", xclbin.stem().string().c_str());
            }
            return;
        }

        std::printf("\n=== %s  (shape %zu개)\n", xclbin.stem().string().c_str(), shapes.size());
        count.ran += shapes.size();
        count.passed += hw::run_gemm(xclbin, shapes, how);
        return;
    }

    std::printf("\n=== %s\n  '%s' 을 돌리는 케이스가 아직 없다\n",
                xclbin.stem().string().c_str(),
                op.c_str());
}

} // namespace

int
main(int argc, char** argv)
{
    hw::options how{};
    how.dir = "../lnpu-artifacts/amd";

    bool        dir_given = false, repeats_given = false;
    std::size_t bad       = 0;

    for (int at = 1; at < argc; ++at)
    {
        std::string_view const arg{argv[at]};

        if (arg == "--dry")
        {
            how.dry = true;
        }
        else if (arg == "--isolate")
        {
            how.isolate = true;
        }
        else if (arg == "--drain")
        {
            how.drain = true;
        }
        else if (arg == "--verify")
        {
            if (at + 1 >= argc)
            {
                std::printf("--verify 뒤에 횟수가 필요하다\n");
                return 2;
            }
            how.verify = std::max(1, std::atoi(argv[++at]));
        }
        else if (arg == "--order")
        {
            if (at + 1 >= argc)
            {
                std::printf("--order 뒤에 쉼표로 구분한 목록이 필요하다\n");
                return 2;
            }

            std::string_view listed{argv[++at]};
            while (not listed.empty())
            {
                auto const comma = listed.find(',');
                auto const one   = listed.substr(0, comma);
                if (not one.empty()) how.order.emplace_back(one);
                if (comma == std::string_view::npos) break;
                listed.remove_prefix(comma + 1);
            }
        }
        else if (arg == "--only")
        {
            if (at + 1 >= argc)
            {
                std::printf("--only 뒤에 문자열이 필요하다\n");
                return 2;
            }
            how.only = argv[++at];
        }
        else if (arg.starts_with("--"))
        {
            std::printf("모르는 옵션: %.*s\n", static_cast<int>(arg.size()), arg.data());
            ++bad;
        }
        else if (not dir_given)
        {
            how.dir   = arg;
            dir_given = true;
        }
        else if (not repeats_given)
        {
            how.repeats   = std::atoi(argv[at]);
            repeats_given = true;
        }
        else
        {
            std::printf("남는 인자: %.*s\n", static_cast<int>(arg.size()), arg.data());
            ++bad;
        }
    }

    if (bad > 0)
    {
        std::printf("사용법: nex-hwtest [디렉터리] [반복] [--dry] [--isolate] [--only <문자열>]\n");
        return 2;
    }

    if (not fs::is_directory(how.dir))
    {
        std::printf("아티팩트 디렉터리가 없다: %s\n", how.dir.string().c_str());
        return 2;
    }

    std::printf("아티팩트  %s\n", fs::absolute(how.dir).string().c_str());
    if (not how.only.empty()) std::printf("케이스    '%s' 를 담은 것만\n", how.only.c_str());
    if (how.isolate) std::printf("격리      shape 마다 hw_context 를 새로 연다 (진단용)\n");
    if (how.drain) std::printf("드레인    입력 전송까지 기다린다 (aiecc 스트림과 달라진다)\n");
    if (how.verify != 2) std::printf("검증      shape 마다 %d 회 실행하고 전부 잰다\n", how.verify);
    if (not how.order.empty())
    {
        std::printf("순서      ");
        for (auto const& one : how.order) std::printf("%s ", one.c_str());
        std::printf("\n");
    }

    std::vector<fs::path> designs{};
    for (auto const& entry : fs::directory_iterator{how.dir})
    {
        if (entry.path().extension() == ".xclbin") designs.push_back(entry.path());
    }
    std::sort(designs.begin(), designs.end());

    tally count{};
    for (auto const& xclbin : designs)
    {
        try
        {
            run_one(xclbin, how, count);
        }
        catch (std::exception const& failure)
        {
            std::printf("  예외: %s\n", failure.what());
        }
    }

    std::printf("\n%zu개 중 %zu개 통과\n", count.ran, count.passed);
    return count.passed == count.ran ? 0 : 1;
}
