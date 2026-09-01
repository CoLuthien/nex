
#include "operation-registry.hpp"

#include <spdlog/spdlog.h>

#include <aiebu/aiebu.h>
#include <xrt/xrt_device.h>

#include <cstdlib>

namespace lnpu::nex::amd
{

operation_registry::operation_registry(std::shared_ptr<xrt::device> device) //
    : m_device(device),                                                     //
      m_operations()
{
}

std::error_code
operation_registry::register_operation(std::string_view operation_name, xrt::xclbin&& xclbin)
{
    if (count() >= kMaximumXclbinCount)
    {
        return std::make_error_code(std::errc::address_not_available);
    }

    std::error_code code{};
    try
    {
        auto [_, success] = m_operations.emplace(operation_name,
                                                 std::make_unique<operation>(operation::parameters{
                                                     .device = m_device,
                                                     .xclbin = std::move(xclbin),
                                                 }));

        if (not success)
        {
            spdlog::error("kernel name {} is already registered.", operation_name);
            code = std::make_error_code(std::errc::already_connected);
        }
    }
    catch (std::exception const& e)
    {
        spdlog::error(e.what());
        code = std::make_error_code(std::errc::bad_address);
    }

    return code;
}

operation*
operation_registry::find_operation(std::string_view name)
{
    auto iter = m_operations.find(name);
    if (iter == m_operations.end())
    {
        return nullptr;
    }

    auto& [key, operation] = *iter;

    return operation.get();
}

} // namespace lnpu::nex::amd
