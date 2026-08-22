
#include "device.hpp"

#include <stdexcept>
#include <string>

namespace lnpu::nex::amd
{

device::device(int device_id)
    : m_device(std::make_shared<xrt::device>(device_id)),
      m_registry(std::make_unique<operation_registry>(m_device))
{
}

std::error_code
device::load_op(std::string_view key, xrt::xclbin&& binary)
{
    return m_registry->register_operation(key, std::move(binary));
}

operation*
device::op(std::string_view key)
{
    return m_registry->find_operation(key);
}

} // namespace lnpu::nex::amd
