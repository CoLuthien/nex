
#pragma once

#include "cmn-api.hpp"
#include "cmn/encoded/encoded.hpp"

namespace lnpu
{

class CMN_EXPORT encoded::slice final
{
    encoded::format m_format;
    buffer::shared  m_buffer;

public:
    explicit slice(encoded::format format, buffer::shared buffer);
    ~slice() = default;

    slice(slice const&)            = delete;
    slice& operator=(slice const&) = delete;

    slice(slice&&)            = default;
    slice& operator=(slice&&) = default;

    std::byte*       ptr();
    std::byte const* ptr() const;

    template <number T>
    T* ptr()
    {
        return reinterpret_cast<T*>(ptr());
    }

    template <number T>
    T const* ptr() const
    {
        return reinterpret_cast<T const*>(ptr());
    }

    template <number T>
    encoded::shared cast() const;

    template <number T>
    void cast(encoded::shared buffer) const;

    slice take(layout_type::shape_container const& from, layout_type::shape_container const& to);
    slice take(layout_type::shape_container const& to);

    encoded::shared contiguous() const;

    buffer::shared source() const;

    layout_type::rank_type rank() const;
    std::size_t            n_element() const;
    std::size_t            n_bytes() const;
    EDataType              dtype() const;

    layout_type::shape_container const&  shape() const;
    layout_type::stride_container const& stride() const;

    layout_type::extent_type shape(layout_type::rank_type dim) const;
    layout_type::extent_type stride(layout_type::rank_type dim) const;

    layout_type   layout() const;
    format const& fmt() const;

    /// @copydoc encoded::is_quantized
    bool is_quantized() const;

    /// @copydoc encoded::scale
    std::span<float const>        scale() const;
    std::span<std::int32_t const> zero_point() const;
};

} // namespace lnpu
