
#pragma once

#include "cmn-api.hpp"
#include <memory>

namespace nex
{

class CMN_EXPORT buffer : public std::enable_shared_from_this<buffer>
{
public:
    using shared       = std::shared_ptr<buffer>;
    using descriptor_t = int;

    class view;

    buffer()          = default;
    virtual ~buffer() = default;

    buffer(buffer&&)            = delete;
    buffer& operator=(buffer&&) = delete;

    buffer(buffer const&)            = delete;
    buffer& operator=(buffer const&) = delete;

    // public:
    virtual std::size_t size_bytes() const = 0;

    /**
     * @brief Byte distance from the start of the underlying allocation to what ptr() hands out.
     *        Zero for a buffer that owns its memory; a view reports where it was anchored.
     */
    virtual std::size_t offset() const { return 0; }

    // public:
    virtual std::byte const* ptr() const = 0;
    virtual std::byte*       ptr()       = 0;

    /**
     * @brief A window of @p size_bytes bytes starting @p offset_bytes into this buffer.
     *
     * Nothing is copied: the window hands out an address inside this allocation and holds a
     * reference to it, so the bytes stay alive for as long as the window does. This is how a
     * strided sub-region is anchored at its own first element -- see encoded::slice.
     *
     * @throws std::out_of_range when the window does not fit this buffer.
     */
    std::shared_ptr<view> make_view(std::size_t offset_bytes, std::size_t size_bytes);
};

class CMN_EXPORT buffer::view final : public buffer
{
    std::size_t    m_offset_bytes;
    std::size_t    m_size_bytes;
    buffer::shared m_source;

public:
    using shared = std::shared_ptr<view>;

    view(buffer::shared source, std::size_t offset_bytes, std::size_t size_bytes);
    ~view() override = default;

    std::size_t size_bytes() const override { return m_size_bytes; }
    std::size_t offset() const override { return m_offset_bytes; }

    std::byte const* ptr() const override;
    std::byte*       ptr() override;

    /// @brief The buffer this window was cut from.
    buffer::shared source() const { return m_source; }
};

} // namespace nex
