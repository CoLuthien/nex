
#pragma once

#include "operation.hpp"
#include "operation-registry.hpp"

#include "nex/backend/device.hpp"

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
class device : public nex::device, //
               public std::enable_shared_from_this<device>
{
    std::shared_ptr<xrt::device> m_device;
    operation_registry::unique   m_registry;

public:
    explicit device(int device_id);

    network::shared create_network(std::string_view            name,
                                   network_description::shared description) override;

    std::error_code load_op(std::string_view key, xrt::xclbin&& binary);
    operation*      op(std::string_view key);
};

} // namespace lnpu::nex::amd
