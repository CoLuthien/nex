

#include "linux-file.hpp"

#ifdef __linux__

extern "C"
{
#include <fcntl.h>    // open, O_RDONLY, O_RDWR
#include <sys/mman.h> // mmap, munmap
#include <sys/stat.h> // fstat
#include <unistd.h>   // close
}

namespace nex
{

file::mapped_file::mapped_file(file::shared file, int flags) : m_file(file)
{
    int prot_flags = PROT_READ;
    if (file->mode() == EAccessMode::ReadWrite)
    {
        prot_flags |= PROT_WRITE;
    }

    m_mapped = ::mmap(nullptr, //
                      file->size_bytes(),
                      prot_flags,
                      MAP_SHARED | flags,
                      file->descriptor(),
                      0);

    if (MAP_FAILED == m_mapped)
    {
        throw std::system_error(errno,
                                std::generic_category(),
                                "mmap failed for fd=" + std::to_string(file->descriptor()));
    }
}

} // namespace nex

#endif
