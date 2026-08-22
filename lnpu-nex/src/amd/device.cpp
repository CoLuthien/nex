
#include "device.hpp"

namespace lnpu::nex::amd
{

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
