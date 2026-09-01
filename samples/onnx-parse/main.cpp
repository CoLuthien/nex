
// Reads a real quantized export through the onnx frontend and then asks whether what came back is
// a graph anything could be lowered from.
//
// Parsing that does not throw is not the same as parsing that worked. A model of this size arrives
// in three files -- the graph, its weights, and the encodings that say how those weights are
// quantized -- and each of them can be read successfully while still being paired with the wrong
// other two: a window taken at the wrong offset of a mapped weight file is bytes either way, and
// encodings belonging to another export name the same tensors. So the sample loads the model and
// then goes back over it through the same public interface a backend would: every operand
// resolves, nothing is read before it is written, every weight is reachable both by name and by
// index and its bytes can be read to the last element, and every scale describes a code range the
// values it belongs to actually fall in.
//
// Run it against an export directory:
//
//     ./sample-onnx-parse ~/workspace/lm-exporter/out/gemma4-quant/chunk_0/chunk_0.seq1.onnx
//
// The weights and the encodings beside the model are found by name; --help lists the rest.

#include "artifact.hpp"
#include "verify.hpp"

#include <spdlog/spdlog.h>

#include <chrono>
#include <cstdio>
#include <exception>
#include <vector>

int
main(int argc, char** argv)
{
    sample::options how{};
    if (not sample::arrange(argc, argv, how))
    {
        return 2;
    }

    // The loader warns about everything it had to guess at, which is a great deal on a graph this
    // size and drowns the report. --verbose is for when one of those guesses is the question.
    spdlog::set_level(how.verbose ? spdlog::level::trace : spdlog::level::err);

    sample::announce(how);

    lnpu::nex::network_description::shared graph{};

    auto const started = std::chrono::steady_clock::now();
    try
    {
        graph = lnpu::nex::nx::load(how.source);
    }
    catch (std::exception const& failure)
    {
        std::printf("\nthe model did not load: %s\n", failure.what());
        return 1;
    }

    auto const took =
        std::chrono::duration<double>{std::chrono::steady_clock::now() - started}.count();

    std::printf("loaded in %.2f s\n", took);

    sample::summarise(*graph);
    sample::list_layers(*graph, how.listed);

    sample::depth const how_far{.sampled = how.sampled};

    std::vector<sample::verdict> const results{
        sample::check_interface(*graph),
        sample::check_value_table(*graph),
        sample::check_operands(*graph),
        sample::check_dependency_order(*graph),
        sample::check_signatures(*graph),
        sample::check_parameters(*graph, how_far),
        sample::check_quantization(*graph),
        sample::check_encodings_fit_weights(*graph, how_far),
    };

    return sample::conclude(results) ? 0 : 1;
}
