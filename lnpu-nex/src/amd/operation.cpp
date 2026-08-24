
#include "operation.hpp"

#include "program-registry.hpp"

#include <aiebu/aiebu.h>

#include <xrt/experimental/xrt_xclbin.h>

#include <algorithm>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <ranges>

namespace lnpu::nex::amd
{

namespace
{

// mlir-aie and IRON emit a single PS kernel per xclbin whose name starts with
// this. The full name carries an instance suffix (e.g. "MLIR_AIE:MLIRAIE"), so
// the prefix is matched instead of the whole string.
constexpr std::string_view k_kernel_prefix{"MLIR_AIE"};

/**
 * @brief The raw USER_METADATA section, or nothing when the xclbin has none.
 *
 * xrt::xclbin exposes sections it knows about and USER_METADATA is not one of them, so the axlf
 * header is walked directly. The payload is not owned: it points into the xclbin's own buffer,
 * which outlives the parse that immediately follows.
 */
std::string_view
user_metadata(xrt::xclbin const& binary)
{
    auto const* top = binary.get_axlf();
    if (top == nullptr) return {};

    for (std::uint32_t at = 0; at < top->m_header.m_numSections; ++at)
    {
        auto const& header = top->m_sections[at];
        if (header.m_sectionKind != static_cast<std::uint32_t>(USER_METADATA)) continue;

        return {reinterpret_cast<char const*>(top) + header.m_sectionOffset,
                static_cast<std::size_t>(header.m_sectionSize)};
    }

    return {};
}

void
generate_elf(std::span<command::word const> instructions, void** elf, std::size_t* elf_size)
{
    uint32_t elf_buf_size =
        aiebu_assembler_get_elf(aiebu_assembler_buffer_type_blob_instr_transaction,
                                reinterpret_cast<char const*>(instructions.data()),
                                instructions.size() * sizeof(command::word),
                                NULL,
                                0,
                                elf,
                                NULL,
                                0,
                                "",
                                "",
                                NULL,
                                0);
    *elf_size = elf_buf_size;
}

} // namespace

descriptor
operation::read_descriptor(xrt::xclbin const& binary)
{
    auto const section = user_metadata(binary);
    if (section.empty())
    {
        throw std::runtime_error(
            "[nex::amd::operation] the xclbin carries no design descriptor. It was not baked by "
            "lnpu-kernels, or it was baked before the descriptor was embedded");
    }

    // xclbinutil pads a RAW section out to an alignment, and the padding is not json.
    auto const text = section.substr(0, section.find('\0'));

    return descriptor::parse(text);
}

operation::operation(parameters&& param) //
    : m_device(std::move(param.device)), //
      m_descriptor(read_descriptor(param.xclbin))
{
    if (nullptr == m_device)
    {
        throw std::invalid_argument("[nex::amd::operation] device must not be null");
    }

    auto& xclbin = param.xclbin;

    auto const kernels = xclbin.get_kernels();
    auto const found   = std::ranges::find_if(kernels, [](xrt::xclbin::kernel const& kernel) {
        return kernel.get_name().starts_with(k_kernel_prefix);
    });

    if (found == kernels.end())
    {
        throw std::runtime_error("[nex::amd::operation] xclbin declares no kernel named '" +
                                 std::string{k_kernel_prefix} + "...'");
    }

    m_kernel_name = found->get_name();

    // Two steps rather than device.load_xclbin(): the NPU shim does not
    // implement that path. register_xclbin() publishes the metadata to the
    // driver, and the hardware context is then opened against its uuid. The
    // context is what binds every instance created from this operation to the
    // array configuration this xclbin describes.
    auto uuid           = m_device->register_xclbin(xclbin);
    m_operation_context = std::make_shared<xrt::hw_context>(*m_device, uuid);
}

std::unique_ptr<operation::instance>
operation::create_instance(layer_description::shared description, std::error_code& ec)
{
    ec = {};

    if (nullptr == description)
    {
        ec = std::make_error_code(std::errc::invalid_argument);
        return nullptr;
    }

    auto const* how = find_lowering(description->op_type());
    if (nullptr == how)
    {
        spdlog::error("[nex::amd::operation] no program runs '{}'", description->op_type());
        ec = std::make_error_code(std::errc::function_not_supported);
        return nullptr;
    }

    // Caught here rather than inside the program: a gemm layer handed an rmsnorm xclbin is a
    // mistake about which operation was opened, and saying so in those words is more use than the
    // program's own "this design is not mine".
    if (how->design_op != m_descriptor.op())
    {
        spdlog::error("[nex::amd::operation] '{}' needs a '{}' design and this xclbin is '{}'",
                      description->op_type(),
                      how->design_op,
                      m_descriptor.op());
        ec = std::make_error_code(std::errc::invalid_argument);
        return nullptr;
    }

    auto const program = how->build(m_descriptor, *description, ec);
    if (ec) return nullptr;

    auto const common = m_descriptor.common();
    auto sequence     = std::make_unique<command_list>(common.generation, common.partition_columns);

    if ((ec = program->wire(*sequence)))
    {
        spdlog::error(
            "[nex::amd::operation] '{}' would not lower: {}", description->name(), ec.message());
        return nullptr;
    }

    return create_instance(std::move(sequence));
}

std::unique_ptr<operation::instance>
operation::create_instance(command_list::unique commands)
{
    return std::make_unique<operation::instance>(
        m_kernel_name, m_operation_context, std::move(commands));
}

operation::instance::instance(std::string                      kernel_name,
                              std::shared_ptr<xrt::hw_context> context,
                              command_list::unique             commands)
    : m_kernel_name(std::move(kernel_name)), //
      m_commands(std::move(commands)),       //
      m_context(context)
{
    void*       elf{nullptr};
    std::size_t elf_size{0};

    // we assume that commands are finalized, but make sure that command list is finalized
    if (not m_commands->finalize())
    {
        throw std::runtime_error("command finalization failed");
    }

    generate_elf(m_commands->as_instructions(), &elf, &elf_size);

    m_executable = std::make_unique<xrt::elf>(elf, elf_size);
    m_module     = std::make_unique<xrt::module>(*m_executable);
    m_kernel     = std::make_unique<xrt::ext::kernel>(*m_context, *m_module, m_kernel_name);

    std::free(elf);
}
} // namespace lnpu::nex::amd
