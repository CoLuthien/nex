#pragma once

#include "cmn/encoded/encoded.hpp"
#include "cmn/encoded/encoded-slice.hpp"

namespace nex::detail
{

/**
 * @brief Builds the sub-region [@p from, @p to) of the region described by @p parent, whose
 *        element zero is the address @p buffer hands out.
 *
 * Both bounds are coordinates in the parent's own index space, half-open as usual: dimension d
 * of the result has extent to[d] - from[d]. Strides are inherited untouched, so the result
 * addresses the very same elements as the parent -- nothing is copied.
 *
 * Per the layout contract of strided_copy_bytes(), the displacement of the sub-region is carried
 * by the buffer rather than by the layout: the returned slice holds a buffer::view anchored at
 * its first element, and its layout keeps offset 0.
 *
 * A per-channel quantization follows the sub-region: when the bounds narrow its channel axis, the
 * slice is given the scale / zero_point entries of the channels it kept -- as views onto the
 * parent's, so nothing is copied here either. The parameters of a region whose count does not
 * describe its own axis are passed on untouched, with a warning; a per-tensor quantization needs
 * no narrowing and is carried over as is.
 *
 * @throws std::invalid_argument if the bounds do not have the parent's rank, or the encoding has
 *         no fixed element size.
 * @throws std::out_of_range if [from[d], to[d]) is not a sub-range of dimension d, or if the
 *         resulting displacement falls outside @p buffer.
 */
encoded::slice take(encoded::format const&        parent,
                    buffer::shared const&         buffer,
                    layout::shape_container const& from,
                    layout::shape_container const& to);

} // namespace nex::detail
