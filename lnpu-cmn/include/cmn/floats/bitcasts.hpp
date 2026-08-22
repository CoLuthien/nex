#pragma once

#include <bit>
#include <cstdint>
#include <limits>

namespace lnpu::detail
{

static_assert(sizeof(float) == sizeof(std::uint32_t));
static_assert(sizeof(double) == sizeof(std::uint64_t));
static_assert(std::numeric_limits<float>::is_iec559,
              "fp32 bitcasts require IEEE-754 binary32 semantics.");
static_assert(std::numeric_limits<double>::is_iec559,
              "fp64 bitcasts require IEEE-754 binary64 semantics.");

[[nodiscard]] inline constexpr float
fp32_from_bits(std::uint32_t bits) noexcept
{
    return std::bit_cast<float>(bits);
}

[[nodiscard]] inline constexpr std::uint32_t
fp32_to_bits(float value) noexcept
{
    return std::bit_cast<std::uint32_t>(value);
}

[[nodiscard]] inline constexpr double
fp64_from_bits(std::uint64_t bits) noexcept
{
    return std::bit_cast<double>(bits);
}

[[nodiscard]] inline constexpr std::uint64_t
fp64_to_bits(double value) noexcept
{
    return std::bit_cast<std::uint64_t>(value);
}

static_assert(fp32_to_bits(1.0F) == 0x3F800000U);
static_assert(fp32_from_bits(0x3F800000U) == 1.0F);
static_assert(fp64_to_bits(1.0) == UINT64_C(0x3FF0000000000000));
static_assert(fp64_from_bits(UINT64_C(0x3FF0000000000000)) == 1.0);

} // namespace lnpu::detail
