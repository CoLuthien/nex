#pragma once

#include "amd/program.hpp"

#include <cstdint>
#include <string_view>
#include <system_error>

namespace lnpu::nex::amd
{
class descriptor;
}

namespace lnpu::nex::amd::programs
{

/**
 * @brief Root-mean-square normalization over a contiguous run of elements.
 *
 * The design splits the run evenly across its cores and each core normalizes its own slice, so
 * there is nothing to reduce across cores and no accumulation order to preserve: the whole
 * operation is one round of "hand every core its slice, take the slices back". That is why this
 * is the first program -- it exercises descriptor layout, argument patching, token handling and
 * completion waits without any of the tiling a GEMM needs.
 *
 * Shim placement is ours, not the compiler's. Each shim column offers two channels per direction,
 * so the streams are laid down in order: core i reads through column i/2 on channel i%2, the
 * weight streams continue that same sequence where the inputs left off, and the results come back
 * on the matching S2MM channels. Nothing requires this to be the arrangement aiecc chose -- the
 * cores are fed by fifos and do not care which shim channel filled them -- and where it happens
 * to agree, the reference streams shipped beside the artifacts still match byte for byte, which
 * is a free regression test rather than a constraint.
 */
class rmsnorm final : public program
{
public:
    /// The descriptor's "op". A design that says anything else was baked for another program.
    static constexpr std::string_view kOp{"rmsnorm"};

    /**
     * @brief What the xclbin fixed for this operator.
     *
     * #elements is here rather than with the shape because the design fixes it. A core loops
     * elements / (columns * channels * tile) times and that count is a constant in the IRON
     * design, not a runtime parameter, so one xclbin normalizes one length. Handing it a longer
     * run does not fail -- the first slice is computed and the rest is left as it was, which
     * reads as a partly-correct answer. Moving that count into an RTP is what makes a design
     * shape-agnostic, and is why the field is expected to outlive the constraint.
     */
    struct design
    {
        amd::design common;

        /// Streams a column carries, which is cores per column for the unweighted design.
        std::uint32_t channels{1};

        /// What one core normalizes in one pass, and the total the design was baked for.
        std::uint32_t tile{};
        std::uint32_t elements{};

        /// Whether the design multiplies by a learned weight, which needs a stream of its own.
        bool weighted{};

        /// Width of one stored element. bf16 is 2.
        std::uint32_t element_bytes{};
    };

    /**
     * @brief Reads a design out of an xclbin's descriptor.
     *
     * @throws std::runtime_error when the descriptor is for another operator, or is missing a
     *         field this needs. Guessing at an absent field would run and answer wrongly.
     */
    static design describe(descriptor const& metadata);

    /// What the call decides: where the buffers are, and how long the run is. The length is
    /// checked against the design rather than taken from it, so picking the wrong xclbin for a
    /// layer is refused instead of quietly normalizing the wrong extent.
    struct parameters
    {
        std::uint32_t elements{};

        binding input;
        binding weight; ///< Ignored unless the design is weighted.
        binding output;
    };

    rmsnorm(design fixed, parameters param);

    /**
     * @throws nothing. A shape the design cannot run is refused rather than lowered into commands
     *         that would be accepted by the hardware and quietly compute the wrong thing.
     */
    std::error_code wire(command_list& sequence) const override;

    std::uint32_t buffer_descriptors_used() const override;

    /// Cores the design puts to work: columns * channels.
    std::uint32_t cores() const;

    /// Elements handed to one core.
    std::uint32_t slice() const;

private:
    design     m_design;
    parameters m_param;
};

} // namespace lnpu::nex::amd::programs
