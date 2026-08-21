#include "cmn/encoded/encoded-slice.hpp"
#include "cmn/buffer/mapped-buffer.hpp"

#include "quantization.hpp"
#include "strided-copy.hpp"
#include "take.hpp"

#include <spdlog/spdlog.h>

#include <system_error>
#include <utility>

namespace nex
{

encoded::slice::slice(encoded::format format, buffer::shared buffer)
    : m_format(std::move(format)), //
      m_buffer(std::move(buffer))
{
}

encoded::slice
encoded::slice::take(layout_type::shape_container const& from,
                     layout_type::shape_container const& to)
{
    return detail::take(m_format, m_buffer, from, to);
}

encoded::slice
encoded::slice::take(layout_type::shape_container const& to)
{
    return detail::take(m_format, m_buffer, layout_type::shape_container(to.size(), 0), to);
}

// The buffer is a view anchored at this slice's first element, so no offset is applied here --
// see the layout contract in strided-copy.hpp.
std::byte*
encoded::slice::ptr()
{
    return m_buffer->ptr();
}

std::byte const*
encoded::slice::ptr() const
{
    return m_buffer->ptr();
}

buffer::shared
encoded::slice::source() const
{
    return m_buffer;
}

encoded::layout_type::rank_type
encoded::slice::rank() const
{
    return m_format.layout.rank();
}

std::size_t
encoded::slice::n_element() const
{
    return m_format.layout.element_count();
}

std::size_t
encoded::slice::n_bytes() const
{
    return m_format.layout.element_count() * in_bytes(m_format.encoding);
}

EDataType
encoded::slice::dtype() const
{
    return m_format.encoding;
}

encoded::layout_type::shape_container const&
encoded::slice::shape() const
{
    return m_format.layout.shape();
}

encoded::layout_type::stride_container const&
encoded::slice::stride() const
{
    return m_format.layout.strides();
}

encoded::layout_type::extent_type
encoded::slice::shape(layout_type::rank_type dim) const
{
    return m_format.layout.extent(dim);
}

encoded::layout_type::extent_type
encoded::slice::stride(layout_type::rank_type dim) const
{
    return m_format.layout.stride(dim);
}

encoded::layout_type
encoded::slice::layout() const
{
    return m_format.layout;
}

encoded::format const&
encoded::slice::fmt() const
{
    return m_format;
}

bool
encoded::slice::is_quantized() const
{
    return detail::is_quantized(m_format);
}

std::span<float const>
encoded::slice::scale() const
{
    return detail::scale_of(m_format);
}

std::span<std::int32_t const>
encoded::slice::zero_point() const
{
    return detail::zero_point_of(m_format);
}

encoded::shared
encoded::slice::contiguous() const
{
    auto format   = m_format;
    format.layout = nex::layout{layout().shape()};

    std::error_code ec{};
    auto            buffer = allocate_mapped(format.layout, format.encoding, ec);

    if (ec)
    {
        spdlog::critical("[encoded::slice] failed to allocate the dense copy of '{}': {}",
                         format.name,
                         ec.message());
        return nullptr;
    }

    auto result = encoded::create(format, buffer);

    encoded::copy(*this, *result);

    return result;
}

} // namespace nex
