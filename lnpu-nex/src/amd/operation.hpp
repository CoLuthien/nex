
#pragma once

#include "command.hpp"
#include "descriptor.hpp"
#include "design.hpp"

#include "nex/frontend/layer-description.hpp"

#include <xrt/xrt_device.h>
#include <xrt/xrt_hw_context.h>

#include <xrt/experimental/xrt_kernel.h>
#include <xrt/experimental/xrt_ext.h>
#include <xrt/experimental/xrt_module.h>
#include <xrt/experimental/xrt_elf.h>

#include <spdlog/spdlog.h>

#include <concepts>
#include <cstddef>
#include <memory>
#include <span>
#include <system_error>
#include <vector>

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

    class executable;

    /**
     * @brief Lowers one layer onto this design and makes it runnable.
     *
     * The whole path in one call: the operator's lowering is looked up by what the node calls
     * itself, that lowering builds the program, the program writes its commands, and the stream
     * becomes an ELF the array can be given. Nothing here branches on which operator it is --
     * find_lowering() answers that with a hash lookup, and the builder it hands back is the only
     * place a program's concrete type has to be named.
     *
     * Where the buffers are is not decided here. A program's bindings name the argument slots the
     * design declares, and which buffer is bound to a slot is settled by set_input()/set_output()
     * on the network instance, so one lowered layer serves whatever the caller binds to it.
     *
     * @param ec why the layer was refused: no lowering for its op_type, a design baked for
     *        another operator, or a shape this one cannot run. All three are ordinary answers
     *        while walking a graph, so none of them throws.
     *
     * @return nullptr with @p ec set when the layer cannot be run on this design.
     */
    std::unique_ptr<executable> create_instance(layer_description::shared description,
                                                std::error_code&          ec);

    /**
     * @brief Makes a stream that is already written runnable.
     *
     * The way in for a caller that built its program by hand rather than from a graph -- a shape
     * sweep, a reference test -- and the step create_instance(layer_description) finishes with.
     *
     * @throws std::runtime_error when the stream will not finalize or aiebu will not assemble it.
     */
    std::unique_ptr<executable> create_instance(command_list::unique commands);
};

class operation::executable final
{
    command_list::unique m_commands;
    std::string          m_kernel_name;

    std::shared_ptr<xrt::hw_context>  m_context;
    std::unique_ptr<xrt::elf>         m_executable;
    std::unique_ptr<xrt::module>      m_module;
    std::unique_ptr<xrt::ext::kernel> m_kernel;

    std::vector<binding> m_bindings;

public:
    using unique = std::unique_ptr<operation::executable>;

    explicit executable(std::string                      kernel_name,
                        std::shared_ptr<xrt::hw_context> context,
                        command_list::unique             commands,
                        std::vector<binding>             bindings = {});

    /// The operands the program lowered, for a caller that wants to bind them by name. Empty for
    /// a stream built by hand, which has no graph behind it and is run positionally instead.
    std::span<binding const> bindings() const { return m_bindings; }

    /**
     * @brief Registers caller-owned storage as a kernel argument, without allocating or copying.
     *
     * The bytes belong to whoever allocated them -- a graph's memory planner hands out slices of
     * one arena -- and the returned object is a view the driver can resolve an address from. It
     * must not outlive the storage it names.
     *
     * @throws std::invalid_argument for empty storage, and whatever XRT throws for storage it
     *         will not take. A host pointer XRT cannot pin is a refusal, not a wrong answer.
     */
    xrt::bo wrap_argument(void* data, std::size_t bytes) const;

    /**
     * @brief Runs the operation once and waits for it, with the buffers given by argument index.
     *
     * @param parameters indexed the way descriptor::argument() counts -- 0 is the design's first
     *        declared buffer, not the kernel's first argument. The three the kernel takes ahead
     *        of those are supplied here, so a caller never restates them.
     */
    std::error_code execute(std::span<xrt::bo> parameters);

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
    requires(std::same_as<Args, xrt::bo> and ...)
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
