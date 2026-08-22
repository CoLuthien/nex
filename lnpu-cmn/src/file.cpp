
#include "cmn/file.hpp"
#include "cmn/buffer/buffer-interface.hpp"

#include "linux-file.hpp"

#include <stdexcept>
#include <system_error>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else
extern "C"
{
#include <fcntl.h>    // open, O_RDONLY, O_RDWR
#include <sys/mman.h> // mmap, munmap
#include <sys/stat.h> // fstat
#include <unistd.h>   // close
}
#endif

namespace lnpu
{

file::shared
file::load(fs::path const& path, int open_flag, EAccessMode mode)
{
    if (not fs::exists(path))
    {
        return nullptr;
    }

    auto size = fs::file_size(path);
    if (size == 0)
    {
        return nullptr;
    }

#ifdef _WIN32
    throw std::runtime_error("Unimplemented");
    DWORD desired_access =
        (mode == EAccessMode::READ_WRITE) ? (GENERIC_READ | GENERIC_WRITE) : GENERIC_READ;
    DWORD share_mode = FILE_SHARE_READ;
    DWORD creation   = OPEN_EXISTING;
    DWORD flags      = FILE_ATTRIBUTE_NORMAL;

    // Note: open_flag is ignored on Windows for now
    (void)open_flag;

    HANDLE handle = ::CreateFileW(
        path.wstring().c_str(), desired_access, share_mode, nullptr, creation, flags, nullptr);

    if (handle == INVALID_HANDLE_VALUE)
    {
        return nullptr;
    }

    return std::make_shared<file>(handle, size, mode, File::Passkey{});

#else // POSIX
    int access_flag = (mode == EAccessMode::ReadOnly) ? O_RDONLY : O_RDWR;

    auto fd = ::open(path.c_str(), access_flag | open_flag);

    if (fd == -1)
    {
        return nullptr;
    }

    return std::make_shared<file>(fd, size, mode, pass_key{});
#endif
}

file::file(file_handle_t handle, std::size_t size, EAccessMode mode, pass_key)
    : m_handle(handle),   //
      m_size_bytes(size), //
      m_mode(mode)
{
}

buffer::shared
file::map(int flags)
{
    try
    {
        return std::make_shared<mapped_file>(shared_from_this(), flags);
    }
    catch (std::exception const&)
    {
        return nullptr;
    }
}

} // namespace lnpu
