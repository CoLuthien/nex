#pragma once

#include "cmn/numeric-types.hpp"
#include "cmn/floats/bitcasts.hpp"

#include <bit> // std::countl_zero
#include <cstdint>

/*
 * IEEE binary16 (half / f16) <-> IEEE binary32 (fp32) conversions.
 *
 * f16 layout:   1 sign | 5 exponent (bias 15) | 10 mantissa
 * fp32 layout:  1 sign | 8 exponent (bias 127) | 23 mantissa
 *
 * Unlike bf16 (whose exponent field matches fp32 bit-for-bit, so conversion is a
 * pure truncation -- see bf16.hpp), f16 has a NARROWER, DIFFERENTLY-BIASED
 * exponent. Conversion therefore has to re-bias the exponent, renormalize half
 * denormals into fp32 normals on the way up, and re-round/re-denormalize on the
 * way down. The routines below are adapted from the well-tested PyTorch/FP16
 * (Marat Dukhan) implementations.
 *
 * as_f32bits_f16 is integer/bitwise only. as_f16 uses IEEE-like fp32 arithmetic
 * (multiplies/adds, no assumptions about denormals) plus bitcasts; both remain
 * constexpr under C++20 (bit_cast + std::countl_zero + core fp arithmetic).
 */
namespace lnpu::detail
{

namespace f16_impl
{
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using i32 = std::int32_t;
} // namespace f16_impl

/*
 * f16 -> fp32, in bit representation. No floating-point operations.
 *
 * Handles every input class exactly: normals, half denormals (renormalized into
 * fp32 normals), +/-0, inf and NaN (mantissa carried into the fp32 payload).
 */
[[nodiscard]] inline constexpr std::uint32_t
as_f32bits_f16(std::uint16_t bits) noexcept
{
    using namespace f16_impl;

    // Shift the half into the high 16 bits: [ S | EEEEE | MM MMMM MMMM | 0..0 ].
    const u32 w       = static_cast<u32>(bits) << 16U;
    const u32 sign    = w & 0x80000000U;
    const u32 nonsign = w & 0x7FFFFFFFU;

    /*
     * renorm_shift: how far to shift a denormal mantissa left so its leading 1
     * lands in the exponent field (turning it into a normalized fp32). Normals
     * have a set bit in the high 6 bits already, so the shift clamps to 0.
     * std::countl_zero(0) == 32 is well-defined; the zero case is masked out
     * below regardless.
     */
    u32 renorm_shift = static_cast<u32>(std::countl_zero(nonsign));
    renorm_shift     = renorm_shift > 5U ? renorm_shift - 5U : 0U;

    /*
     * inf_nan_mask == 0x7F800000 iff the half exponent was 0x1F (inf/NaN),
     * else 0. Adding 0x04000000 overflows a max half-exponent into bit 31, and
     * the arithmetic >> 8 smears it across the fp32 exponent field.
     */
    const i32 inf_nan_mask = (static_cast<i32>(nonsign + 0x04000000U) >> 8) & 0x7F800000;

    /*
     * zero_mask == 0xFFFFFFFF iff nonsign == 0 (i.e. +/-0), else 0. nonsign - 1
     * underflows to 0xFFFFFFFF only when nonsign is 0; the arithmetic >> 31
     * broadcasts the sign bit.
     */
    const i32 zero_mask = static_cast<i32>(nonsign - 1U) >> 31;

    /*
     * Normalize (<< renorm_shift), pack the exponent+mantissa into fp32
     * position (>> 3), rebias the exponent by (0x70 - renorm_shift) at bit 23,
     * force the exponent to all-ones on inf/NaN, zero everything on +/-0, then
     * re-apply the sign.
     */
    return sign | ((((nonsign << renorm_shift >> 3) + ((0x70U - renorm_shift) << 23U)) |
                    static_cast<u32>(inf_nan_mask)) &
                   ~static_cast<u32>(zero_mask));
}

/** Convert half bits to float. (fp32_from_bits is a bitcast, not an FP op.) */
[[nodiscard]] inline constexpr float
as_f32(f16 value) noexcept
{
    return fp32_from_bits(as_f32bits_f16(value.value));
}

/*
 * fp32 -> f16, round-to-nearest-even (RNE).
 *
 * Adapted from PyTorch/FP16 fp16_ieee_from_fp32_value. The standard trick:
 * scale |x| up by 2**112 then down by 2**110 so the rounding of the fp32 result
 * lands exactly at the f16 mantissa boundary (the hardware rounder does RNE for
 * us), then reassemble exponent+mantissa. fabsf is replaced by a sign-clearing
 * bitcast so the whole thing stays constexpr. NaN maps to a quiet NaN (0x7E00),
 * with the input sign preserved (so -NaN -> 0xFE00).
 */
[[nodiscard]] inline constexpr f16
as_f16(float value) noexcept
{
    using namespace f16_impl;

    const float scale_to_inf  = fp32_from_bits(0x77800000U); // 2**112
    const float scale_to_zero = fp32_from_bits(0x08800000U); // 2**-110

    const u32   w         = fp32_to_bits(value);
    const u32   sign      = w & 0x80000000U;
    const float abs_value = fp32_from_bits(w & 0x7FFFFFFFU); // |value|, no FP op

    float base = (abs_value * scale_to_inf) * scale_to_zero;

    const u32 shl1_w = w + w;
    u32       bias   = shl1_w & 0xFF000000U;
    if (bias < 0x71000000U)
    {
        bias = 0x71000000U;
    }

    base = fp32_from_bits((bias >> 1) + 0x07800000U) + base;

    const u32 result_bits   = fp32_to_bits(base);
    const u32 exp_bits      = (result_bits >> 13) & 0x00007C00U;
    const u32 mantissa_bits = result_bits & 0x00000FFFU;
    const u32 nonsign       = exp_bits + mantissa_bits;

    // shl1_w > 0xFF000000 <=> exponent all-ones with non-zero mantissa (NaN).
    const u16 out =
        static_cast<u16>((sign >> 16) | (shl1_w > 0xFF000000U ? 0x7E00U : nonsign));
    return static_cast<f16>(out);
}

// --- up-conversion: representative bit patterns --------------------------------
static_assert(as_f32bits_f16(0x0000U) == 0x00000000U);   // +0
static_assert(as_f32bits_f16(0x8000U) == 0x80000000U);   // -0
static_assert(as_f32bits_f16(0x3C00U) == 0x3F800000U);   // 1.0
static_assert(as_f32bits_f16(0xBC00U) == 0xBF800000U);   // -1.0
static_assert(as_f32bits_f16(0x4000U) == 0x40000000U);   // 2.0
static_assert(as_f32bits_f16(0x0001U) == 0x33800000U);   // smallest denormal, 2**-24
static_assert(as_f32bits_f16(0x03FFU) == 0x387FC000U);   // largest denormal
static_assert(as_f32bits_f16(0x7C00U) == 0x7F800000U);   // +inf
static_assert(as_f32bits_f16(0xFC00U) == 0xFF800000U);   // -inf
static_assert((as_f32bits_f16(0x7E00U) & 0x7F800000U) == 0x7F800000U); // NaN exp
static_assert((as_f32bits_f16(0x7E00U) & 0x007FFFFFU) != 0U);          // NaN mantissa

static_assert(as_f32(static_cast<f16>(0x3C00U)) == 1.0F);

// --- down-conversion: round-to-nearest-even ------------------------------------
static_assert(as_f16(0.0F).value == 0x0000U);
static_assert(as_f16(1.0F).value == 0x3C00U);
static_assert(as_f16(-1.0F).value == 0xBC00U);
static_assert(as_f16(2.0F).value == 0x4000U);
static_assert(as_f16(fp32_from_bits(0x7F800000U)).value == 0x7C00U); // +inf
static_assert(as_f16(fp32_from_bits(0xFF800000U)).value == 0xFC00U); // -inf
// NaN -> 0x7E00 is verified at runtime: FP ops that yield NaN are not a core
// constant expression, so as_f16(NaN) cannot appear in a static_assert.
// round-trip of every exactly-representable value used above
static_assert(as_f16(as_f32(static_cast<f16>(0x3C00U))).value == 0x3C00U);
static_assert(as_f16(as_f32(static_cast<f16>(0x0001U))).value == 0x0001U);
static_assert(as_f16(as_f32(static_cast<f16>(0x03FFU))).value == 0x03FFU);

} // namespace lnpu::detail
