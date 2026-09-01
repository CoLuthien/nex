
#pragma once

#include "operation.hpp"

#include "cmn/containers/maps.hpp"

#include <xrt/xrt_device.h>
#include <xrt/xrt_hw_context.h>

#include <cstddef>
#include <memory>
#include <unordered_map>

namespace lnpu::nex::amd
{

class operation_registry
{
    std::unordered_map<std::string, operation::unique, string_hash, string_equal> m_operations;
    std::shared_ptr<xrt::device>                                                  m_device;

    static constexpr auto kMaximumXclbinCount = 16; // This is hard constraint from the XRT driver
public:
    using unique = std::unique_ptr<operation_registry>;

    explicit operation_registry(std::shared_ptr<xrt::device> device);

    std::size_t count() const { return m_operations.size(); }

    std::error_code register_operation(std::string_view operation_name, xrt::xclbin&& xclbin);
    operation*      find_operation(std::string_view name);
};

} // namespace lnpu::nex::amd
