
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

/**
 * @brief The zero points of @p entry, negated into our convention.
 *
 * AIMET dequantizes as (code + offset) * scale, we as (code - zero_point) * scale, so one is the
 * negation of the other.
 */
std::vector<std::int32_t>
zero_points_of(json const& entry, std::size_t expected, std::string const& name)
{
    auto const& listed = entry.at("offset");
    if (not listed.is_array() or listed.size() != expected)
    {
        throw std::runtime_error("[nx::encodings] '" + name + "' has " +
                                 std::to_string(listed.is_array() ? listed.size() : 0) +
                                 " offset(s) against " + std::to_string(expected) + " scale(s)");
    }

    std::vector<std::int32_t> zero_points{};
    zero_points.reserve(listed.size());
    for (auto const& offset : listed)
    {
        zero_points.emplace_back(-offset.get<std::int32_t>());
    }

    return zero_points;
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
    parameters.scale = as_parameter(scales, EDataType::f32, per_channel, name + " [scale]");

    // A symmetric quantization has no zero point to store: every entry would be zero, which is
    // what an absent zero_point already means.
    if (not symmetric)
    {
        parameters.zero_point = as_parameter(zero_points_of(entry, scales.size(), name),
                                             EDataType::i32,
                                             per_channel,
                                             name + " [zero_point]");
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
