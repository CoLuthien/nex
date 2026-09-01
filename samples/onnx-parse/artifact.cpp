
#include "artifact.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string_view>

namespace sample
{
namespace
{

/// "chunk_0.seq1.onnx" -> "chunk_0.seq1"; the encodings of a model are named after this.
std::string
stem_of(fs::path const& model)
{
    auto name = model.filename().generic_string();
    if (name.ends_with(".onnx"))
    {
        name.resize(name.size() - std::string_view{".onnx"}.size());
    }

    return name;
}

/**
 * @brief The weight files a model exported in pieces reads from.
 *
 * TensorProto::external_data names them by file name and nothing else, so which of the files
 * beside the model belong to it is answered the same way: a model that reads a file spells its
 * name somewhere in the message. That matters here because a graph rewritten from another one
 * often keeps pointing at the weights of the model it came from -- "chunk_0.seq1.onnx" reads
 * "chunk_0.onnx.data" -- so the conventional "<model>.data" is a guess, not an answer.
 *
 * Should the model name none of them, every sibling is offered rather than none: handing the
 * loader one too many costs an address-space mapping and nothing else, while leaving the right
 * one out costs the load.
 */
std::vector<fs::path>
weights_beside(fs::path const& model)
{
    std::ifstream     reading{model, std::ios::binary};
    std::string const message{std::istreambuf_iterator<char>{reading},
                              std::istreambuf_iterator<char>{}};

    std::vector<fs::path> beside{};
    std::vector<fs::path> named{};

    for (auto const& entry : fs::directory_iterator{model.parent_path()})
    {
        auto const name = entry.path().filename().generic_string();
        if (not name.ends_with(".onnx.data"))
        {
            continue;
        }

        beside.emplace_back(entry.path());
        if (message.find(name) != std::string::npos)
        {
            named.emplace_back(entry.path());
        }
    }

    auto& chosen = named.empty() ? beside : named;
    std::sort(chosen.begin(), chosen.end());

    return chosen;
}

std::size_t
number(char const* text, std::size_t fallback)
{
    char*      end   = nullptr;
    auto const value = std::strtoull(text, &end, 10);

    return (end == text) ? fallback : static_cast<std::size_t>(value);
}

void
usage(char const* program)
{
    std::printf(
        "usage: %s <model.onnx> [options]\n"
        "\n"
        "  --data <file>        a weight file the model reads from; repeatable. Defaults to the\n"
        "                       sibling '*.onnx.data' files the model names.\n"
        "  --encodings <file>   an AIMET encodings.json. Defaults to '<stem>.encodings' beside\n"
        "                       the model; pass an empty path for a model that is not quantized.\n"
        "  --list <n>           print the first n layers of the graph (default 12).\n"
        "  --sample <n>         elements per weight the value checks read (default 4096);\n"
        "                       0 reads every one of them.\n"
        "  --verbose            let the loader's own warnings through.\n",
        program);
}

} // namespace

bool
arrange(int argc, char** argv, options& how)
{
    bool encodings_given = false;

    for (int at = 1; at < argc; ++at)
    {
        std::string_view const argument{argv[at]};
        auto const next = [&]() -> char const* { return at + 1 < argc ? argv[++at] : nullptr; };

        if (argument == "-h" or argument == "--help")
        {
            usage(argv[0]);
            return false;
        }
        else if (argument == "--verbose")
        {
            how.verbose = true;
        }
        else if (argument == "--data")
        {
            auto const* path = next();
            if (nullptr == path) break;
            how.source.external_data.emplace_back(path);
        }
        else if (argument == "--encodings")
        {
            auto const* path = next();
            if (nullptr == path) break;
            how.source.encodings = path;
            encodings_given      = true;
        }
        else if (argument == "--list")
        {
            auto const* count = next();
            if (nullptr == count) break;
            how.listed = number(count, how.listed);
        }
        else if (argument == "--sample")
        {
            auto const* count = next();
            if (nullptr == count) break;
            how.sampled = number(count, how.sampled);
        }
        else if (argument.starts_with("-"))
        {
            std::printf("unknown option '%s'\n\n", argv[at]);
            usage(argv[0]);
            return false;
        }
        else if (how.source.model.empty())
        {
            how.source.model = argument;
        }
        else
        {
            std::printf("only one model at a time; '%s' is the second\n", argv[at]);
            return false;
        }
    }

    if (how.source.model.empty())
    {
        usage(argv[0]);
        return false;
    }

    // A directory is what one has at hand after an export, so take the single model in it rather
    // than making the caller spell the name out. More than one is ambiguous and stays an error.
    if (fs::is_directory(how.source.model))
    {
        std::vector<fs::path> models{};
        for (auto const& entry : fs::directory_iterator{how.source.model})
        {
            if (entry.path().extension() == ".onnx")
            {
                models.emplace_back(entry.path());
            }
        }

        if (models.size() != 1)
        {
            std::printf("'%s' holds %zu onnx models; name the one to read\n",
                        how.source.model.generic_string().c_str(),
                        models.size());
            return false;
        }

        how.source.model = models.front();
    }

    if (not fs::is_regular_file(how.source.model))
    {
        std::printf("no model at '%s'\n", how.source.model.generic_string().c_str());
        return false;
    }

    if (how.source.external_data.empty())
    {
        how.source.external_data = weights_beside(how.source.model);
    }

    if (not encodings_given)
    {
        auto const beside =
            how.source.model.parent_path() / (stem_of(how.source.model) + ".encodings");
        if (fs::exists(beside))
        {
            how.source.encodings = beside;
        }
    }

    return true;
}

void
announce(options const& how)
{
    std::printf("model      : %s (%.1f MiB)\n",
                how.source.model.generic_string().c_str(),
                static_cast<double>(fs::file_size(how.source.model)) / (1024.0 * 1024.0));

    for (auto const& weights : how.source.external_data)
    {
        std::printf("weights    : %s (%.1f MiB)\n",
                    weights.generic_string().c_str(),
                    static_cast<double>(fs::file_size(weights)) / (1024.0 * 1024.0));
    }

    if (how.source.encodings.empty())
    {
        std::printf("encodings  : none -- the model is read as a float graph\n");
    }
    else
    {
        std::printf("encodings  : %s (%.1f MiB)\n",
                    how.source.encodings.generic_string().c_str(),
                    static_cast<double>(fs::file_size(how.source.encodings)) / (1024.0 * 1024.0));
    }
}

} // namespace sample
