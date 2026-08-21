
#pragma once

#include "cmn/file.hpp"

#ifdef __linux__

namespace nex
{

class file::mapped_file final : public buffer
{
    file::shared m_file{nullptr};
    void*        m_mapped{nullptr};

public:
    mapped_file(file::shared file, int flags);
    ~mapped_file() override;

    std::size_t size_bytes() const override { return m_file->size_bytes(); }

    std::byte const* ptr() const override { return reinterpret_cast<std::byte const*>(m_mapped); }
    std::byte*       ptr() override { return reinterpret_cast<std::byte*>(m_mapped); };
};
} // namespace nex

#endif
