#include "cmn/encoded/encoded.hpp"
#include "cmn/encoded/encoded-slice.hpp"

#include "quantization.hpp"
#include "strided-copy.hpp"
#include "take.hpp"

#include "cmn/buffer/mapped-buffer.hpp"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <array>
#include <cstring>
#include <stdexcept>
#include <string>
#include <system_error>
#include <utility>

namespace nex
{

namespace
{

/**
 * @brief Whether @p parameter holds one entry per channel of an axis @p channels wide, laid out so
 *        that a range of channels is a range of entries.
 *
 * How the parameters are STORED is settled when the region is built -- detail::validate()
 * normalizes them to f32 / i32 -- but not that their count describes this axis, which is what
 * makes a channel range meaningful. A region whose parameters already disagree with its own axis is
 * left exactly as it was found: there is no honest sub-range to cut out of it.
 */
bool
describes_channels(encoded::shared const& parameter, std::size_t channels)
{
    // 1-D is the shape a per-channel parameter is documented to have (see encoded::quantization);
    // it is also what makes the range below a range of one dimension.
    return parameter                              //
           and parameter->rank() == 1             //
           and parameter->n_element() == channels //
           and parameter->layout().is_contiguous();
}

/**
 * @brief The channels [@p first, @p first + @p count) of a per-channel parameter region, as a view
 *        onto the very same entries.
 *
 * The parameters are a region like any other, so their sub-range is taken the same way -- which is
 * also what keeps this allocation-free where it matters: detail::take() copies nothing, and the
 * buffer::view it anchors holds a reference to the buffer behind @p parameter, so the entries stay
 * alive for as long as the slice that reads them.
 *
 * The nesting stops here. A slice is not a region, so the sub-range is rebuilt as one, and what it
 * is rebuilt from carries no quantization of its own -- a scale has no scale.
 */
encoded::shared
narrow(encoded::shared const& parameter, std::size_t first, std::size_t count)
{
    auto const entries = detail::take(parameter->fmt(),
                                      parameter->source(),
                                      {static_cast<layout::extent_type>(first)},
                                      {static_cast<layout::extent_type>(first + count)});

    // The sub-range of a 1-D contiguous region is contiguous and anchored at its own first entry,
    // which is the layout an encoded is built with.
    auto format = entries.fmt();
    format.name += '[' + std::to_string(first) + ':' + std::to_string(first + count) + ')';

    return encoded::create(std::move(format), entries.source());
}

} // namespace

namespace detail
{

encoded::slice
take(encoded::format const&         parent,
     buffer::shared const&          buffer,
     layout::shape_container const& from,
     layout::shape_container const& to)
{
    auto const& source = parent.layout;
    auto const  rank   = source.rank();

    if (from.size() != rank or to.size() != rank)
    {
        throw std::invalid_argument("encoded::take: rank mismatch (layout=" + std::to_string(rank) +
                                    ", from=" + std::to_string(from.size()) +
                                    ", to=" + std::to_string(to.size()) + ")");
    }

    if (not has_fixed_element_size(parent.encoding))
    {
        throw std::invalid_argument("encoded::take: dtype " + to_string(parent.encoding) +
                                    " has no fixed element size");
    }
    auto const elem_size = in_bytes(parent.encoding);

    layout::shape_container shape(rank);

    // Distance from the parent's element zero to the sub-region's, and the extent the sub-region
    // reaches from there -- both in elements, both accumulated over the parent's own strides.
    std::size_t displacement = 0;
    std::size_t span         = 1;

    for (layout::rank_type d = 0; d < rank; ++d)
    {
        if (from[d] < 0 or to[d] < from[d] or to[d] > source.extent(d))
        {
            throw std::out_of_range("encoded::take: [" + std::to_string(from[d]) + ", " +
                                    std::to_string(to[d]) + ") is not a sub-range of dim " +
                                    std::to_string(d) + " (extent " +
                                    std::to_string(source.extent(d)) + ")");
        }

        auto const stride = static_cast<std::size_t>(source.stride(d));

        shape[d] = to[d] - from[d];
        displacement += static_cast<std::size_t>(from[d]) * stride;
        span += static_cast<std::size_t>(shape[d] - 1) * stride;
    }

    auto sliced = nex::layout{std::move(shape), source.strides(), 0};

    // An empty sub-region has no first element to anchor at; leaving it on the parent's base
    // keeps the view inside the allocation, and nothing may be dereferenced through it anyway.
    auto const empty        = sliced.element_count() == 0;
    auto const offset_bytes = empty ? 0 : displacement * elem_size;
    auto const capacity     = buffer->size_bytes();

    if (offset_bytes >= capacity and not empty)
    {
        throw std::out_of_range("encoded::take: sub-region starts at byte " +
                                std::to_string(offset_bytes) + " of a " + std::to_string(capacity) +
                                " byte buffer; the layout does not describe '" + parent.name + "'");
    }

    // Clamped rather than trusted: span is only the true reach of the sub-region for the
    // non-negative strides a layout is built with here.
    auto const size_bytes = empty ? 0 : std::min(span * elem_size, capacity - offset_bytes);

    auto format = parent;

    // A per-channel parameter is indexed by the coordinate along its axis -- that is what
    // resolve_affine() pairs them by -- so the parameters the sub-region needs are exactly the
    // channels it keeps, in the order it keeps them. Handing on the parent's instead would leave a
    // count that resolve_affine() refuses, turning every later cast<T>() of the slice into a throw.
    if (format.quant and format.quant->axis and *format.quant->axis < rank)
    {
        auto const axis     = *format.quant->axis;
        auto const channels = static_cast<std::size_t>(source.extent(axis));
        auto const kept     = static_cast<std::size_t>(sliced.extent(axis));

        auto const scale      = format.quant->scale;
        auto const zero_point = format.quant->zero_point;

        // The two are narrowed together or not at all: a scale that no longer matches its own zero
        // point would be worse than parameters that plainly describe the parent.
        auto const narrowable = describes_channels(scale, channels) and
                                (not zero_point or describes_channels(zero_point, channels));

        if (kept != channels and narrowable)
        {
            auto const first = static_cast<std::size_t>(from[axis]);

            format.quant->scale = narrow(scale, first, kept);
            if (zero_point)
            {
                format.quant->zero_point = narrow(zero_point, first, kept);
            }
        }
        else if (kept != channels)
        {
            spdlog::warn("[encoded::take] '{}' is quantized per channel along axis {}, which the "
                         "sub-region narrows from {} to {}, but its scale/zero_point do not "
                         "describe that axis to begin with; they are passed on as they are",
                         format.name,
                         axis,
                         channels,
                         kept);
        }
    }

    format.layout = std::move(sliced);

    return encoded::slice{std::move(format), buffer->make_view(offset_bytes, size_bytes)};
}

} // namespace detail

namespace
{

// The same fixed dimension limit strided_copy works under.
constexpr std::size_t kMaxDims = 8;

using dim_array = std::array<std::size_t, kMaxDims>;

/**
 * @brief Moves every retained chunk from the byte offset it sits at to the one it now belongs to.
 *
 * The walk is an odometer over @p extent: each step advances the innermost dimension that has not
 * run out and carries into the next, keeping both offsets in step with the coordinate. Running it
 * @p descending covers the case where a chunk lands past where it came from -- the ground ahead
 * has already been read by then, so nothing is overwritten before it is moved.
 */
void
move_chunks(std::byte*       base,
            std::size_t      chunk,
            std::size_t      rank,
            dim_array const& extent,
            dim_array const& src_stride, // bytes
            dim_array const& dst_stride, // bytes
            bool             descending)
{
    dim_array   coord{};
    std::size_t total = 1;
    std::size_t src   = 0;
    std::size_t dst   = 0;

    for (std::size_t d = 0; d < rank; ++d)
    {
        if (extent[d] == 0) return;
        total *= extent[d];

        if (descending)
        {
            coord[d] = extent[d] - 1;
            src += coord[d] * src_stride[d];
            dst += coord[d] * dst_stride[d];
        }
    }

    for (std::size_t i = 0; i < total; ++i)
    {
        if (dst != src) std::memmove(base + dst, base + src, chunk);

        for (auto d = rank; d-- > 0;)
        {
            if (descending)
            {
                if (coord[d] > 0)
                {
                    --coord[d];
                    src -= src_stride[d];
                    dst -= dst_stride[d];
                    break;
                }

                coord[d] = extent[d] - 1;
                src += coord[d] * src_stride[d];
                dst += coord[d] * dst_stride[d];
            }
            else
            {
                if (++coord[d] < extent[d])
                {
                    src += src_stride[d];
                    dst += dst_stride[d];
                    break;
                }

                coord[d] = 0;
                src -= (extent[d] - 1) * src_stride[d];
                dst -= (extent[d] - 1) * dst_stride[d];
            }
        }
    }
}

} // namespace

encoded::encoded(format&& format, buffer::shared buffer)
    : m_buffer(std::move(buffer)),                     //
      m_format(std::forward<encoded::format>(format)), //
      m_initial_layout(m_format.layout),               //
      m_initial_quant(m_format.quant)
{
    // A dynamic layout is not a strided view: is_contiguous() says false about it only because
    // unknown extents cannot be checked, not because anything is strided. Graph descriptions
    // arrive that way -- a partial shape becomes -1 -- and their extents are resolved by
    // reshape() before anything is read through the region.
    if (not m_format.layout.is_dynamic() and not m_format.layout.is_contiguous())
    {
        throw std::logic_error("encoded '" + m_format.name +
                               "' was given a layout that is not contiguous; only a slice may be "
                               "strided");
    }

    // Normalized here, once, so that reading the parameters downstream costs a pointer cast
    // rather than a dispatch -- see scale() / zero_point().
    if (m_format.quant)
    {
        detail::validate(*m_format.quant, m_format.name);
    }
}

encoded::shared
encoded::create(struct format format, buffer::shared buffer)
{
    return std::make_shared<encoded>(std::move(format), std::move(buffer));
}

void
encoded::copy(encoded const& from, encoded& to)
{
    detail::strided_copy(from, to);
}

void
encoded::copy(encoded const& from, slice& to)
{
    detail::strided_copy(from, to);
}

void
encoded::copy(slice const& from, encoded& to)
{
    detail::strided_copy(from, to);
}

void
encoded::copy(slice const& from, slice& to)
{
    detail::strided_copy(from, to);
}

encoded::slice
encoded::take(layout_type::shape_container const& from,
              layout_type::shape_container const& to) const
{
    return detail::take(m_format, m_buffer, from, to);
}

encoded::slice
encoded::take(layout_type::shape_container const& to) const
{
    return detail::take(m_format, m_buffer, layout_type::shape_container(to.size(), 0), to);
}

encoded::slice
encoded::as_slice() const
{
    // The whole region, so there is nothing to displace: whatever the buffer hands out is already
    // element zero, and the layout is passed on as is.
    return encoded::slice{m_format, m_buffer};
}

void
encoded::reshape(nex::layout layout, bool compaction)
{
    auto const  rank    = m_format.layout.rank();
    auto const& target  = layout.shape();
    auto const& current = m_format.layout.shape();

    if (target.size() != rank)
    {
        throw std::invalid_argument(
            "encoded::reshape: rank mismatch (layout=" + std::to_string(rank) +
            ", requested=" + std::to_string(target.size()) + ")");
    }

    if (not has_fixed_element_size(m_format.encoding))
    {
        throw std::invalid_argument("encoded::reshape: dtype " + to_string(m_format.encoding) +
                                    " has no fixed element size");
    }
    auto const elem_size = in_bytes(m_format.encoding);

    // The shape the region was built with is what its buffer was sized for, so it is also how far
    // each dimension may grow back. A dynamic one never described a real capacity, and then only
    // the buffer itself can bound the request.
    auto const  bounded  = not m_initial_layout.is_dynamic();
    auto const& capacity = m_initial_layout.shape();

    for (layout_type::rank_type d = 0; d < rank; ++d)
    {
        if (target[d] < 0)
        {
            throw std::invalid_argument("encoded::reshape: dim " + std::to_string(d) + " of '" +
                                        m_format.name + "' was given the dynamic extent " +
                                        std::to_string(target[d]));
        }

        if (bounded and target[d] > capacity[d])
        {
            throw std::out_of_range("encoded::reshape: dim " + std::to_string(d) + " of '" +
                                    m_format.name + "' holds at most " +
                                    std::to_string(capacity[d]) + ", requested " +
                                    std::to_string(target[d]));
        }
    }

    if (auto const size_bytes = layout.element_count() * elem_size;
        size_bytes > m_buffer->size_bytes())
    {
        throw std::out_of_range("encoded::reshape: " + std::to_string(size_bytes) +
                                " bytes do not fit the " + std::to_string(m_buffer->size_bytes()) +
                                " byte buffer behind '" + m_format.name + "'");
    }

    if (target == current) return;

    // Both sides of the compaction are dense: the constructor turns away a region that is not,
    // and every shape handed back below is contiguous.
    if (compaction) compact_inplace(current, target);

    // The parameters of the channels the new shape has room for -- cut from the ones the region was
    // built with, never from the ones the last reshape() left it. Both directions are then just a
    // prefix of the same parameters, so an axis that shrinks and grows back comes out of it exactly
    // as it went in; narrowing in place would have thrown the outer channels away on the way down.
    if (m_initial_quant                   //
        and m_format.quant                //
        and m_initial_quant->axis         //
        and *m_initial_quant->axis < rank //
        and target[*m_initial_quant->axis] != current[*m_initial_quant->axis])
    {
        auto const axis = *m_initial_quant->axis;
        auto const kept = static_cast<std::size_t>(target[axis]);

        // Unbounded means the axis had no known extent to describe in the first place, so there is
        // no prefix of these parameters to speak of either.
        auto const channels = bounded ? static_cast<std::size_t>(capacity[axis]) : 0;
        auto const narrowable =
            bounded and describes_channels(m_initial_quant->scale, channels) and
            (not m_initial_quant->zero_point or
             describes_channels(m_initial_quant->zero_point, channels));

        if (narrowable)
        {
            m_format.quant->scale = kept == channels //
                                        ? m_initial_quant->scale
                                        : narrow(m_initial_quant->scale, 0, kept);

            if (m_initial_quant->zero_point)
            {
                m_format.quant->zero_point = kept == channels //
                                                 ? m_initial_quant->zero_point
                                                 : narrow(m_initial_quant->zero_point, 0, kept);
            }
        }
        else
        {
            spdlog::warn("[encoded::reshape] '{}' is quantized per channel along axis {}, which the "
                         "new shape takes from {} to {}, but its scale/zero_point do not describe "
                         "the extent the region was built with; they are passed on as they are",
                         m_format.name,
                         axis,
                         current[axis],
                         target[axis]);
        }
    }

    m_format.layout = nex::layout{target};
}

void
encoded::compact_inplace(layout_type::shape_container const& from,
                         layout_type::shape_container const& to)
{
    auto const rank      = from.size();
    auto const elem_size = in_bytes(m_format.encoding);

    // Both sides are read as densely laid out: reshape() has already refused a source that is
    // not, and the shape it hands back always is.
    auto const source = nex::layout{from};
    auto const target = nex::layout{to};

    // What survives the reshape: the part of each dimension both shapes have room for.
    layout_type::shape_container retained(rank);
    for (layout_type::rank_type d = 0; d < rank; ++d)
    {
        retained[d] = std::min(from[d], to[d]);
        if (retained[d] == 0) return; // nothing is kept, so nothing has to move
    }

    // Where a chunk ends up relative to where it starts is the sum of c[d] * (new stride - old
    // stride) over the coordinate. While those differences share a sign the whole move runs one
    // way and a single ordered pass is safe. With mixed signs some chunks move up and others
    // down, no order covers both, and the region goes through scratch space instead.
    bool spreads   = false;
    bool contracts = false;
    for (layout_type::rank_type d = 0; d < rank; ++d)
    {
        if (retained[d] < 2) continue; // a dimension pinned to index 0 displaces nothing

        if (target.stride(d) > source.stride(d))
            spreads = true;
        else if (target.stride(d) < source.stride(d))
            contracts = true;
    }

    if (spreads and contracts)
    {
        std::error_code ec{};
        auto            scratch = allocate_mapped(nex::layout{retained}, m_format.encoding, ec);

        if (ec)
        {
            throw std::runtime_error("encoded::reshape: could not allocate the scratch space to "
                                     "lay '" +
                                     m_format.name + "' out again: " + ec.message());
        }

        auto const dense = nex::layout{retained};

        detail::strided_copy_bytes(m_buffer->ptr(),
                                   nex::layout{retained, source.strides(), 0},
                                   scratch->ptr(),
                                   dense,
                                   m_format.encoding);

        detail::strided_copy_bytes(scratch->ptr(),
                                   dense,
                                   m_buffer->ptr(),
                                   nex::layout{retained, target.strides(), 0},
                                   m_format.encoding);
        return;
    }

    // The innermost dimensions that keep their extent also keep their strides on both sides, so
    // everything they span travels together as one chunk.
    std::size_t chunk = elem_size;
    auto        outer = rank;
    while (outer > 0 and from[outer - 1] == to[outer - 1])
    {
        --outer;
        chunk *= static_cast<std::size_t>(from[outer]);
    }

    if (outer == 0) return; // the shapes differ nowhere that moves an element

    if (outer > kMaxDims)
    {
        throw std::invalid_argument("encoded::reshape: rank " + std::to_string(rank) +
                                    " exceeds the limit of " + std::to_string(kMaxDims));
    }

    dim_array extent{};
    dim_array src_stride{};
    dim_array dst_stride{};

    for (layout_type::rank_type d = 0; d < outer; ++d)
    {
        extent[d]     = static_cast<std::size_t>(retained[d]);
        src_stride[d] = static_cast<std::size_t>(source.stride(d)) * elem_size;
        dst_stride[d] = static_cast<std::size_t>(target.stride(d)) * elem_size;
    }

    move_chunks(m_buffer->ptr(), chunk, outer, extent, src_stride, dst_stride, spreads);
}

std::byte*
encoded::ptr()
{
    return m_buffer->ptr();
}

std::byte const*
encoded::ptr() const
{
    return m_buffer->ptr();
}

buffer::shared
encoded::source() const
{
    return m_buffer;
}

layout::rank_type
encoded::rank() const
{
    return m_format.layout.rank();
}

std::size_t
encoded::n_element() const
{
    return m_format.layout.element_count();
}

std::size_t
encoded::n_bytes() const
{
    return m_format.layout.element_count() * in_bytes(m_format.encoding);
}

EDataType
encoded::dtype() const
{
    return m_format.encoding;
}

layout::shape_container const&
encoded::shape() const
{
    return m_format.layout.shape();
}

layout::stride_container const&
encoded::stride() const
{
    return m_format.layout.strides();
}

layout::extent_type
encoded::shape(layout_type::rank_type dim) const
{
    return m_format.layout.extent(dim);
}

layout::extent_type
encoded::stride(layout_type::rank_type dim) const
{
    return m_format.layout.stride(dim);
}

nex::layout
encoded::layout() const
{
    return m_format.layout;
}

encoded::format const&
encoded::fmt() const
{
    return m_format;
}

bool
encoded::is_quantized() const
{
    return detail::is_quantized(m_format);
}

std::span<float const>
encoded::scale() const
{
    return detail::scale_of(m_format);
}

std::span<std::int32_t const>
encoded::zero_point() const
{
    return detail::zero_point_of(m_format);
}

} // namespace nex
