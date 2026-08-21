#include "cmn/buffer/buffer-interface.hpp"

#include <stdexcept>
#include <string>
#include <utility>

namespace nex
{

buffer::view::view(buffer::shared source, std::size_t offset_bytes, std::size_t size_bytes)
    : m_offset_bytes(offset_bytes), //
      m_size_bytes(size_bytes),     //
      m_source(std::move(source))
{
    if (not m_source)
    {
        throw std::invalid_argument("buffer::view: no buffer to look into");
    }

    // Checked here rather than at every read: a window is handed around long after it was cut,
    // and a reader has no way left to tell whether it still describes its source.
    auto const capacity = m_source->size_bytes();
    if (m_offset_bytes > capacity or m_size_bytes > capacity - m_offset_bytes)
    {
        throw std::out_of_range("buffer::view: [" + std::to_string(m_offset_bytes) + ", " +
                                std::to_string(m_offset_bytes + m_size_bytes) +
                                ") does not fit a " + std::to_string(capacity) + " byte buffer");
    }
}

std::byte const*
buffer::view::ptr() const
{
    auto const* base = m_source->ptr();
    return base == nullptr ? nullptr : base + m_offset_bytes;
}

std::byte*
buffer::view::ptr()
{
    auto* base = m_source->ptr();
    return base == nullptr ? nullptr : base + m_offset_bytes;
}

buffer::view::shared
buffer::make_view(std::size_t offset_bytes, std::size_t size_bytes)
{
    // A window of a window is anchored on the outer allocation, so the chain never grows: the
    // offsets are added up once, here, and a reader walks one indirection whatever it came from.
    if (auto* window = dynamic_cast<view*>(this); window != nullptr)
    {
        return std::make_shared<view>(
            window->source(), window->offset() + offset_bytes, size_bytes);
    }

    return std::make_shared<view>(shared_from_this(), offset_bytes, size_bytes);
}

} // namespace nex
