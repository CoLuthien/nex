#pragma once

#include "cmn/encoded/encoded.hpp"

#include <cstdint>
#include <span>
#include <string>

namespace nex::detail
{

/**
 * @brief Whether @p encoding stores codes that an affine quantization turns into real values.
 *
 * The same rule cast<T>() applies, read off the very trait cast<T>() uses, so the two cannot
 * drift apart: the integer and fixed-point encodings carry a quantization, the real ones do not.
 * An encoding with no fixed element size answers false because nothing can be read through it
 * at all.
 */
bool stores_codes(EDataType encoding);

/**
 * @brief Whether the values @p fmt describes have to be dequantized before they mean anything.
 *
 * Carrying a quantization is not the same as being quantized. On a float or half encoding the
 * parameters are fake-quant metadata -- the stored values are already real, and applying the
 * scale a second time would silently halve them. Only the pairing of parameters WITH an encoding
 * that stores codes makes a region quantized.
 */
bool is_quantized(encoded::format const& fmt);

/**
 * @brief The dequantization parameters of @p fmt, or an empty span when there are none to apply.
 *
 * Empty exactly when is_quantized() is false, so a caller that forgets to ask cannot silently
 * read fake-quant metadata as if it were live. One entry for a per-tensor quantization, one per
 * channel otherwise. zero_point is additionally empty for a symmetric quantization, where every
 * entry would be zero.
 *
 * Both spans point into the parameter regions themselves; they stay valid as long as @p fmt does.
 */
std::span<float const>        scale_of(encoded::format const& fmt);
std::span<std::int32_t const> zero_point_of(encoded::format const& fmt);

/**
 * @brief Rejects a quantization the span accessors could not hand out safely.
 *
 * Scale is normalized to f32 and zero_point to i32 so that reading them costs a pointer cast
 * rather than a dispatch. An importer that has them in another form converts once, at the
 * boundary, with cast<T>(); every reader downstream then gets a plain span.
 *
 * This checks how the parameters are STORED, not whether they describe the region correctly --
 * a per-channel count is only meaningful against an axis, and take() may narrow that axis on
 * purpose. resolve_affine() is where that pairing is checked.
 *
 * @throws std::invalid_argument on a null scale, or on a scale, zero_point or shape that does
 *         not meet the above.
 */
void validate(encoded::quantization const& quant, std::string const& name);

} // namespace nex::detail
