
#pragma once

#include "command.hpp"
#include "descriptor.hpp"

#include <xrt/xrt_device.h>
#include <xrt/xrt_hw_context.h>

#include <xrt/experimental/xrt_kernel.h>
#include <xrt/experimental/xrt_ext.h>
#include <xrt/experimental/xrt_module.h>
#include <xrt/experimental/xrt_elf.h>

#include <spdlog/spdlog.h>

#include <cstddef>
#include <memory>
#include <system_error>

namespace lnpu::nex::amd
{

/**
 * @brief One xclbin, opened on the array and ready to run instruction streams against.
 *
 * The design descriptor the baking script put in the xclbin is read here, once, and handed to
 * whoever builds a program for it. That is the whole reason this owns it: the array arrangement
 * a program has to agree with is a fact about this xclbin, and reading it off the artifact is
 * what keeps the two from disagreeing without anyone noticing.
 */
class operation final
{
    std::shared_ptr<xrt::hw_context> m_operation_context;
    std::shared_ptr<xrt::device>     m_device;
    std::string                      m_kernel_name;
    descriptor                       m_descriptor;

public:
    using unique = std::unique_ptr<operation>;
    struct parameters
    {
        std::shared_ptr<xrt::device> device;
        xrt::xclbin                  xclbin;
    };

    /**
     * @throws std::runtime_error when the xclbin declares no mlir-aie kernel, or carries no
     *         design descriptor. A design nothing can describe is a design nothing can wire,
     *         so it is refused here rather than at the first wrong number.
     */
    explicit operation(parameters&& param);

    operation(operation const&)            = delete;
    operation(operation&&)                 = delete;
    operation& operator=(operation const&) = delete;
    operation& operator=(operation&&)      = delete;

    /**
     * @brief Pulls USER_METADATA out of an xclbin and parses it.
     *
     * Public and static because the descriptor is wanted before there is an operation to ask:
     * a program can be built and its stream checked against a reference without opening the
     * array at all, and there is no reason for that path to grow its own copy of this.
     *
     * @throws std::runtime_error when the section is absent or does not parse.
     */
    static descriptor read_descriptor(xrt::xclbin const& binary);

    /// What the xclbin says about itself. A program's describe() reads its fields out of this.
    descriptor const& metadata() const { return m_descriptor; }

    /**
     * @brief Builds a program against this design.
     *
     * The pairing between a program and the design it was baked for is settled here, and this is
     * the only place it can go wrong: describe() reads the descriptor and refuses one belonging
     * to another operator. After that the program carries its design as a value and the pairing
     * is a fact rather than a convention the caller has to keep.
     *
     * One operation serves as many programs as the model has calls to it -- a single gemm xclbin
     * runs every projection -- so this is called per call site, not once per xclbin.
     *
     * This is convenience, not the only way in: Program{design, parameters} stays public, so a
     * reference test builds and checks the same program from a descriptor literal without an
     * array to open.
     *
     * @throws whatever Program::describe() throws.
     */
    template <typename Program>
    std::shared_ptr<Program> make(typename Program::parameters shape) const
    {
        return std::make_shared<Program>(Program::describe(m_descriptor), std::move(shape));
    }

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

    /**
     * @brief Runs the operation once and waits for it.
     *
     * @param args the buffers, in the order the design's arguments are declared. Pass xrt::bo,
     *        not a subclass: older XRT has no set_arg overload for those and takes them as
     *        scalars, which fails as "patch_value() only supports 64-bit values or less".
     *
     * The leading 3 is ERT_START_NPU. The two zeros after it are the instruction buffer and its
     * length, which this flow does not use -- the module already carries the instructions.
     */
    template <typename... Args>
    std::error_code execute(Args... args)
    {
        auto const state = m_kernel->operator()(3, 0, 0, args...).wait();

        if (state != ERT_CMD_STATE_COMPLETED)
        {
            // The state is the only thing the command tells us, so it is what the caller gets.
            spdlog::error(
                "[nex::amd] '{}' ended in state {}", m_kernel_name, static_cast<int>(state));
            return std::make_error_code(std::errc::io_error);
        }

        return {};
    }
};
} // namespace lnpu::nex::amd
