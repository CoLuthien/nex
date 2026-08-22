
#pragma once

#include "cmn-api.hpp"
#include "cmn/layout.hpp"
#include "cmn/numeric-types.hpp"
#include "cmn/buffer/buffer-interface.hpp"

#include <system_error>

namespace lnpu
{

buffer::shared allocate_mapped(layout const& layout, EDataType type, std::error_code& ec);

class CMN_EXPORT mapped_buffer : public buffer
{
    std::size_t m_size_bytes{0};
    std::byte*  m_data{nullptr};

public:
    mapped_buffer(mapped_buffer const&)            = delete;
    mapped_buffer& operator=(mapped_buffer const&) = delete;
    mapped_buffer(mapped_buffer&&)                 = delete;
    mapped_buffer& operator=(mapped_buffer&&)      = delete;

    explicit mapped_buffer(layout const& layout, EDataType type);
    explicit mapped_buffer(std::size_t size_bytes);

    ~mapped_buffer() override;

    std::size_t size_bytes() const override { return m_size_bytes; }

    // public:
    std::byte const* ptr() const override { return m_data; }
    std::byte*       ptr() override { return m_data; }
};
} // namespace lnpu
