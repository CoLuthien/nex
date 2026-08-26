
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

// Not a nex::device yet, deliberately. That interface promises create_network() and allocate(),
// and neither has anything behind it until the backend network lands -- inheriting it now would
// make this abstract and put a stub where a caller expects a network. The base goes back on in
// the commit that can honour it.
class device
{
    std::shared_ptr<xrt::device> m_device;
    operation_registry::unique   m_registry;

public:
    explicit device(int device_id);

    std::error_code load_op(std::string_view key, xrt::xclbin&& binary);
    operation*      op(std::string_view key);
};

} // namespace lnpu::nex::amd
