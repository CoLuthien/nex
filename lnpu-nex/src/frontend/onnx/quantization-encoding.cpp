
#include "quantization-encoding.hpp"

#include "cmn/buffer/mapped-buffer.hpp"
#include "cmn/encoded/encoded.hpp"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <cstdint>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <system_error>
#include <utility>
#include <vector>

namespace lnpu::nex::nx
{
namespace
{

using json = ::nlohmann::json;

/**
 * @brief A quantization parameter region holding @p values.
 *
 * Rank is what tells the two kinds of quantization apart: a scalar describes the whole tensor,
 * a vector describes one channel each (see encoded::quantization).
 */
template <typename T>
encoded::shared
as_parameter(std::vector<T> const& values, EDataType encoding, bool per_channel, std::string name)
{
    auto shape = per_channel ? layout{static_cast<layout::extent_type>(values.size())} : layout{};

    std::error_code failure{};
    auto            storage = allocate_mapped(shape, encoding, failure);
    if (failure or not storage)
    {
        throw std::runtime_error("[nx::encodings] could not allocate '" + name +
                                 "': " + failure.message());
    }

    std::memcpy(storage->ptr(), values.data(), values.size() * sizeof(T));

    encoded::format description{
        .name     = std::move(name),
        .encoding = encoding,
        .layout   = std::move(shape),
        .quant    = std::nullopt, // a scale has no scale of its own
    };

    return encoded::create(std::move(description), std::move(storage));
}

std::vector<float>
scales_of(json const& entry, std::string const& name)
{
    auto const& listed = entry.at("scale");
    if (not listed.is_array() or listed.empty())
    {
        throw std::runtime_error("[nx::encodings] '" + name +
                                 "' has no scale, or one that is not a list");
    }

    std::vector<float> scales{};
    scales.reserve(listed.size());
    for (auto const& scale : listed)
    {
        scales.emplace_back(scale.get<float>());
    }

    return scales;
}

std::vector<std::int32_t>
offsets_of(json const& entry, std::size_t expected, std::string const& name)
{
    auto const& listed = entry.at("offset");
    if (not listed.is_array() or listed.size() != expected)
    {
        throw std::runtime_error("[nx::encodings] '" + name + "' has " +
                                 std::to_string(listed.is_array() ? listed.size() : 0) +
                                 " offset(s) against " + std::to_string(expected) + " scale(s)");
    }

    std::vector<std::int32_t> offsets{};
    offsets.reserve(listed.size());
    for (auto const& offset : listed)
    {
        offsets.emplace_back(offset.get<std::int32_t>());
    }

    return offsets;
}

std::uint8_t
bitwidth_of(json const& entry, std::string const& name)
{
    auto const bits = entry.at("bw").get<std::int64_t>();

    // encoded::quantization::bitwidth is a uint8_t and detail::validate() bounds it again; this
    // is only here so a nonsense value is named by the file it came from rather than by a region
    // built much later out of it.
    if (bits < 1 or bits > 32)
    {
        throw std::runtime_error("[nx::encodings] '" + name + "' is " + std::to_string(bits) +
                                 " bits wide; a code range is 1 to 32 bits");
    }

    return static_cast<std::uint8_t>(bits);
}

/**
 * @brief Refuses a symmetric entry whose offset is not the one dropping the zero point assumes.
 *
 * This is the check that keeps two storage conventions from being confused for each other, so it
 * is worth spelling out. AIMET dequantizes as (code + offset) * scale over an UNSIGNED code, and
 * a non-strict symmetric range puts offset at -2^(bw-1): code 0 means the most negative value.
 * We drop the zero point instead (see read_entry), which is the same mapping only if the code
 * has been re-stored SIGNED -- shifted down by that same 2^(bw-1).
 *
 * Nothing downstream can notice the difference: every value would simply be 2^(bw-1) * scale too
 * large, which crashes nothing and still looks like a plausible tensor. So the moment the offset
 * stops being the one that shift assumes -- a strict_symmetric export uses -(2^(bw-1) - 1), an
 * unsigned_symmetric one uses 0 -- the file has to be refused rather than read as if it were the
 * layout we know.
 */
void
expect_shifted_offsets(std::vector<std::int32_t> const& offsets,
                       std::uint8_t                     bitwidth,
                       std::string const&               name)
{
    auto const expected = -(std::int32_t{1} << (bitwidth - 1));

    for (std::size_t at = 0; at < offsets.size(); ++at)
    {
        if (offsets[at] != expected)
        {
            throw std::runtime_error(
                "[nx::encodings] '" + name + "' is symmetric at " + std::to_string(bitwidth) +
                " bits, so every offset should be " + std::to_string(expected) + ", but entry " +
                std::to_string(at) + " is " + std::to_string(offsets[at]) +
                "; this export does not use the symmetric range this reader converts");
        }
    }
}

quantization
read_entry(json const& entry, std::string const& name)
{
    auto const kind        = entry.at("enc_type").get<std::string>();
    auto const per_channel = (kind == "PER_CHANNEL");
    auto const symmetric   = entry.value("is_sym", false);

    auto const scales = scales_of(entry, name);

    if (not per_channel and scales.size() != 1)
    {
        throw std::runtime_error("[nx::encodings] '" + name + "' is " + kind + " yet carries " +
                                 std::to_string(scales.size()) +
                                 " scales; only PER_CHANNEL may carry more than one");
    }

    quantization parameters{};
    parameters.bitwidth = bitwidth_of(entry, name);
    parameters.scale    = as_parameter(scales, EDataType::f32, per_channel, name + " [scale]");

    auto const offsets = offsets_of(entry, scales.size(), name);

    if (symmetric)
    {
        // No zero point to store: every entry would be zero, which is what an absent zero_point
        // already means -- but only for a code the converter re-stores signed. Hence the check.
        expect_shifted_offsets(offsets, parameters.bitwidth, name);
    }
    else
    {
        std::vector<std::int32_t> zero_points{};
        zero_points.reserve(offsets.size());
        for (auto const offset : offsets)
        {
            zero_points.emplace_back(-offset);
        }

        parameters.zero_point =
            as_parameter(zero_points, EDataType::i32, per_channel, name + " [zero_point]");
    }

    // AIMET quantizes a parameter per output channel and never says along which axis, because
    // there is only one it ever means.
    if (per_channel)
    {
        parameters.axis = 0;
    }

    return parameters;
}

void
read_section(json const& section, char const* what, quantization_map& into)
{
    if (not section.is_array())
    {
        // 0.6.x wrote an object keyed by name here; read_aimet_encodings() has already refused
        // that layout by version, so anything else is simply not a section
        return;
    }

    for (auto const& entry : section)
    {
        auto const name = entry.at("name").get<std::string>();

        // FLOAT entries mark a value the export left alone; there is no affine mapping to read
        if (entry.value("dtype", std::string{}) != "INT")
        {
            spdlog::debug("[nx::encodings] '{}' is not quantized in the {}", name, what);
            continue;
        }

        if (not into.emplace(name, read_entry(entry, name)).second)
        {
            spdlog::warn("[nx::encodings] '{}' is described twice; keeping the first description",
                         name);
        }
    }
}

} // namespace

quantization_map
read_aimet_encodings(fs::path const& path)
{
    if (not fs::exists(path))
    {
        return {};
    }

    std::ifstream stream{path};
    if (not stream)
    {
        throw std::runtime_error("[nx::encodings] cannot open '" + path.generic_string() + "'");
    }

    json root{};
    try
    {
        stream >> root;
    }
    catch (json::parse_error const& failure)
    {
        throw std::runtime_error("[nx::encodings] '" + path.generic_string() +
                                 "' is not json: " + failure.what());
    }

    // 1.x lists its encodings; 0.6.x kept an object keyed by tensor name, with different field
    // names inside it. Refusing it outright beats reading half of it.
    auto const version = root.value("version", std::string{});
    if (not version.starts_with("1."))
    {
        throw std::runtime_error("[nx::encodings] '" + path.generic_string() + "' is version '" +
                                 version + "'; only AIMET 1.x is read here");
    }

    quantization_map encodings{};

    if (auto const section = root.find("param_encodings"); section != root.end())
    {
        read_section(*section, "parameters", encodings);
    }

    if (auto const section = root.find("activation_encodings"); section != root.end())
    {
        read_section(*section, "activations", encodings);
    }

    spdlog::debug(
        "[nx::encodings] '{}' describes {} value(s)", path.generic_string(), encodings.size());

    return encodings;
}

} // namespace lnpu::nex::nx
