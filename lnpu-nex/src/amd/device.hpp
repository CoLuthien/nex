
#pragma once

#include "operation.hpp"
#include "operation-registry.hpp"

#include <xrt/xrt_device.h>

#include <memory>
#include <string_view>
#include <filesystem>

namespace lnpu::nex::amd
{

namespace fs = std::filesystem;
class device
{
    std::shared_ptr<xrt::device> m_device;
    operation_registry::unique   m_registry;

public:
    explicit device(int device_id);

    /// The XRT device underneath, for buffers that have to be allocated on the same one the
    /// operations were opened against.
    std::shared_ptr<xrt::device> const& handle() const { return m_device; }

    std::error_code load_op(std::string_view key, xrt::xclbin&& binary);
    operation*      op(std::string_view key);
};

} // namespace lnpu::nex::amd
