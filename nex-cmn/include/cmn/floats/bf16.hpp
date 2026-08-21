#pragma once

#include "cmn/numeric-types.hpp"
#include "cmn/floats/bitcasts.hpp"

#include <cstdint>

/*
 * bfloat16 (bf16) <-> IEEE binary32 (fp32) conversions.
 *
 * bf16 layout:  1 sign | 8 exponent | 7 mantissa
 * fp32 layout:  1 sign | 8 exponent | 23 mantissa
 *
 * The exponent field -- both its 8-bit WIDTH and its bias (127) -- is IDENTICAL
 * between bf16 and fp32. A bf16 is therefore literally the high 16 bits of an
 * fp32. Conversion is a pure bit truncation/extension: NONE of the renorm-shift,
 * clz, denormal-magic or exponent-rebias machinery that fp16 needs (fp16 has a
 * 5-bit, differently-biased exponent) is required here.
 *
 * Every function below is integer/bitwise only -- no floating-point arithmetic.
 * They are constexpr because fp32_from_bits/fp32_to_bits are std::bit_cast based.
 */
namespace nex::detail
{

namespace bf16_impl
{
using u16 = std::uint16_t;
using u32 = std::uint32_t;

/** Broadcast a bool into an all-ones / all-zeros 32-bit selector mask. */
[[nodiscard]] constexpr u32
mask_from_bool(bool value) noexcept
{
    return u32{0} - static_cast<u32>(value);
}

/** Branch-free select: bits of when_true where true_mask is set, else when_false. */
[[nodiscard]] constexpr u16
select_u16(u16 when_false, u16 when_true, u32 true_mask) noexcept
{
    const u32 false_value = static_cast<u32>(when_false);
    const u32 true_value  = static_cast<u32>(when_true);

    return static_cast<u16>((false_value & ~true_mask) | (true_value & true_mask));
}
} // namespace bf16_impl

/*
 * bf16 -> fp32, in bit representation. Zero-extend into the high 16 bits.
 *
 * EXACT and lossless for every input class -- normals, denormals, +/-0, inf, and
 * NaN (payload preserved) -- because the result is exactly the fp32 from which
 * this bf16 was truncated.
 */
[[nodiscard]] inline constexpr std::uint32_t
as_f32bits_bf16(std::uint16_t bits) noexcept
{
    return static_cast<std::uint32_t>(bits) << 16U;
}

/** Convert bfloat16 bits to float. (fp32_from_bits is a bitcast, not an FP op.) */
[[nodiscard]] inline constexpr float
as_f32(bf16 value) noexcept
{
    return fp32_from_bits(as_f32bits_bf16(value.value));
}

/*
 * fp32 -> bf16, round-to-nearest-even (RNE). Matches TF/PyTorch/oneDNN bf16.
 *
 * The rounding is a single integer add:
 *     rounding_bias = 0x7FFF + target_lsb
 * When the 16 discarded low bits are exactly 0x8000 (an exact tie), the +lsb
 * term produces a carry ONLY when the kept bit is odd -> round-half-to-even.
 *
 * NaN is handled separately: a NaN whose payload lives entirely in the discarded
 * low 16 bits would truncate to inf, so we force a quiet NaN (exponent stays
 * 0xFF, quiet bit set), preserving the sign.
 */
[[nodiscard]] inline constexpr bf16
as_bf16(float value) noexcept
{
    using namespace bf16_impl;

    const u32 bits  = fp32_to_bits(value);
    const u32 upper = bits >> 16U;

    const u32 target_lsb    = upper & 1U;
    const u32 rounding_bias = 0x7FFFU + target_lsb;
    const u16 rounded       = static_cast<u16>((bits + rounding_bias) >> 16U);

    const u16 quiet_nan = static_cast<u16>(upper | 0x0040U);
    const u32 nan_mask  = mask_from_bool((bits & 0x7FFFFFFFU) > 0x7F800000U);

    return static_cast<bf16>(select_u16(rounded, quiet_nan, nan_mask));
}

/*
 * fp32 -> bf16, round-toward-zero (truncation).
 *
 * Cheaper but biased toward zero. Several NPU/accelerator bf16 store paths
 * truncate instead of RNE; if you must bit-match a specific backend's numerics,
 * pick the mode that backend actually uses. NaN needs the same guard.
 */
[[nodiscard]] inline constexpr bf16
as_bf16_truncate(float value) noexcept
{
    using namespace bf16_impl;

    const u32 bits      = fp32_to_bits(value);
    const u16 truncated = static_cast<u16>(bits >> 16U);
    const u16 quiet_nan = static_cast<u16>(truncated | 0x0040U);
    const u32 nan_mask  = mask_from_bool((bits & 0x7FFFFFFFU) > 0x7F800000U);

    return static_cast<bf16>(select_u16(truncated, quiet_nan, nan_mask));
}

static_assert(as_f32bits_bf16(0x0000U) == 0x00000000U);
static_assert(as_f32bits_bf16(0x8000U) == 0x80000000U);
static_assert(as_f32bits_bf16(0x3F80U) == 0x3F800000U);
static_assert(as_f32(static_cast<bf16>(0x3F80U)) == 1.0F);
static_assert(as_bf16(1.0F).value == 0x3F80U);
static_assert(as_bf16_truncate(1.0F).value == 0x3F80U);
static_assert(as_bf16(fp32_from_bits(0x3F808000U)).value == 0x3F80U);
static_assert(as_bf16(fp32_from_bits(0x3F818000U)).value == 0x3F82U);
static_assert(as_bf16(fp32_from_bits(0x7F800001U)).value == 0x7FC0U);

} // namespace nex::detail
