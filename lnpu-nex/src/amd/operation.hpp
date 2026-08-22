
#pragma once

#include "command.hpp"

#include <xrt/xrt_device.h>
#include <xrt/xrt_hw_context.h>

#include <xrt/experimental/xrt_kernel.h>
#include <xrt/experimental/xrt_ext.h>
#include <xrt/experimental/xrt_module.h>
#include <xrt/experimental/xrt_elf.h>

#include <cstddef>
#include <system_error>
#include <memory>

namespace lnpu::nex::amd
{

class operation final
{
    std::shared_ptr<xrt::hw_context> m_operation_context;
    std::shared_ptr<xrt::device>     m_device;
    std::string                      m_kernel_name;

public:
    using unique = std::unique_ptr<operation>;
    struct parameters
    {
        std::shared_ptr<xrt::device> device;
        xrt::xclbin                  xclbin;
        // TODO
        // need additional information for number of occupying columns;
    };

    // need additional information for number of occupying columns;
    explicit operation(parameters&& param);

    operation(operation const&)            = delete;
    operation(operation&&)                 = delete;
    operation& operator=(operation const&) = delete;
    operation& operator=(operation&&)      = delete;

    class instance;
    std::unique_ptr<instance> create_instance(command_list::unique commands);
};

class operation::instance final
{
    command_list::unique m_commands;
    std::string          m_kernel_name;

    std::shared_ptr<xrt::hw_context>  m_context;
    std::unique_ptr<xrt::elf>         m_executable;
    std::unique_ptr<xrt::module>      m_module;
    std::unique_ptr<xrt::ext::kernel> m_kernel;

public:
    using unique = std::unique_ptr<operation::instance>;

    explicit instance(std::string                      kernel_name,
                      std::shared_ptr<xrt::hw_context> context,
                      command_list::unique             commands);

    template <typename... Args>
    std::error_code execute(Args... args)
    {
        auto run   = m_kernel->operator()(3, 0, 0, args...);
        auto state = run.wait();

        // TODO Error handling and logs;

        return {};
    }
};
} // namespace lnpu::nex::amd
