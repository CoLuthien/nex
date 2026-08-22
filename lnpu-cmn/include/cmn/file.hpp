
#pragma once

#include "cmn-api.hpp"
#include "cmn/buffer/buffer-interface.hpp"

#include <filesystem>
#include <memory>

namespace lnpu
{

#ifdef _WIN32
using file_handle_t = void*; // HANDLE is void* on Windows
#else
using file_handle_t = int;
#endif

namespace fs = std::filesystem;
enum class EAccessMode
{
    ReadOnly,
    ReadWrite,
    WriteOnly,
};

class CMN_EXPORT file : public std::enable_shared_from_this<file>
{
    file_handle_t m_handle;
    std::size_t   m_size_bytes;
    EAccessMode   m_mode;

    // clang-format off
    class pass_key{};
    // clang-format on

    class mapped_file;

public:
    using shared = std::shared_ptr<file>;

    file(file_handle_t handle, std::size_t size, EAccessMode mode, pass_key);
    file(file&&)                 = delete;
    file& operator=(file&&)      = delete;
    file(file const&)            = delete;
    file& operator=(file const&) = delete;

    static file::shared load(fs::path const& path,
                             int             flags,
                             EAccessMode     mode = EAccessMode::ReadOnly);

    buffer::shared map(int flags);

    std::size_t   size_bytes() const { return m_size_bytes; }
    file_handle_t descriptor() const { return m_handle; }
    EAccessMode   mode() const { return m_mode; }
};

} // namespace lnpu
