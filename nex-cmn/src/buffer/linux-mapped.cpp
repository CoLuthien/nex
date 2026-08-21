
#include "cmn/buffer/mapped-buffer.hpp"

#ifdef __linux__

extern "C"
{
#include <sys/mman.h>
}

#include <memory>

namespace nex
{

static std::size_t
buffer_size(layout const& layout, EDataType type)
{
    return layout.element_count() * in_bytes(type);
}

mapped_buffer::mapped_buffer(std::size_t size_bytes) //
    : m_size_bytes(size_bytes)
{
    // mmap refuses a zero-length mapping, and there would be nothing to address through it
    // anyway. An empty region is legal -- a shape with a zero extent, or one still dynamic -- so
    // it is left without memory rather than turned away.
    if (m_size_bytes == 0)
    {
        return;
    }

    void* mapped = ::mmap(nullptr, //
                          m_size_bytes,
                          PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS,
                          -1,
                          0);
    if (mapped == MAP_FAILED)
    {
        throw std::system_error(errno, //
                                std::generic_category(),
                                "anonymous mmap failed for MmapBuffer");
    }

    m_data = static_cast<std::byte*>(mapped);
}

mapped_buffer::mapped_buffer(layout const& layout, EDataType type)
    : mapped_buffer(buffer_size(layout, type))
{
}

mapped_buffer::~mapped_buffer()
{
    if (m_data != nullptr)
    {
        ::munmap(m_data, m_size_bytes);
    }
}

buffer::shared
allocate_mapped(layout const& layout, EDataType type, std::error_code& ec)
{
    ec.clear();

    // An encoding with no fixed element size has no byte count to map, so there is no allocation
    // to attempt -- reported rather than thrown, since every caller here is already error_code
    // based.
    if (not has_fixed_element_size(type))
    {
        ec = std::make_error_code(std::errc::invalid_argument);
        return nullptr;
    }

    try
    {
        return std::make_shared<mapped_buffer>(layout, type);
    }
    catch (std::system_error const& failure)
    {
        ec = failure.code();
        return nullptr;
    }
    catch (std::bad_alloc const&)
    {
        ec = std::make_error_code(std::errc::not_enough_memory);
        return nullptr;
    }
}
} // namespace nex

#endif
