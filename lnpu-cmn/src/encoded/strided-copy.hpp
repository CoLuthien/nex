#pragma once

#include "cmn/encoded/encoded.hpp"
#include "cmn/encoded/encoded-slice.hpp"

#include <cstddef>

namespace lnpu::detail
{

/**
 * @brief Byte-level strided copy between two layouts of identical shape.
 *
 * Each base is the address its owner hands out (encoded::ptr() / slice::ptr()) and is taken as
 * element zero of the corresponding layout. Displacement into a larger allocation is carried by
 * the buffer -- a slice holds a buffer::view that has already resolved it -- so a layout here
 * describes extents and strides only.
 *
 * Neither side needs to be contiguous. Dimensions whose strides agree on BOTH sides are merged
 * first, so the common cases degenerate to a single memcpy.
 *
 * @throws std::invalid_argument on rank/shape mismatch, null base, unknown dtype, or rank above
 *         the fixed dimension limit.
 */
void strided_copy_bytes(std::byte const* src_base,
                        layout const&    src_layout,
                        std::byte*       dst_base,
                        layout const&    dst_layout,
                        EDataType        dtype);

/**
 * @brief Raw element copy between encodeds and slices, in whatever strided form each one has.
 *
 * This moves the STORED values verbatim; it never rescales. The encodings must match, and the
 * two quantizations must agree -- copying uf8 codes into a buffer whose scale differs would
 * silently change the values they stand for. Use encoded::cast<T>() to requantize instead.
 */
void strided_copy(encoded const& src, encoded& dst);
void strided_copy(encoded const& src, encoded::slice& dst);
void strided_copy(encoded::slice const& src, encoded& dst);
void strided_copy(encoded::slice const& src, encoded::slice& dst);

} // namespace lnpu::detail
