#pragma once

#include "amd/program.hpp"

#include <cstdint>
#include <system_error>
#include <vector>

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
 * Shim placement is derived, not given. Each shim column offers two channels per direction, so
 * the streams are laid down in order: core i reads through column i/2 on channel i%2, the weight
 * streams continue that same sequence where the inputs left off, and the results come back on the
 * matching S2MM channels. That rule reproduces what the compiler placed for the smaller designs
 * exactly; where a design's own placer chose otherwise, placement has to come from the design
 * instead (see placement below).
 */
class rmsnorm final : public program
{
public:
    /// Which shim column and channel a stream is carried on.
    struct outlet
    {
        std::uint32_t column{};
        std::uint32_t channel{};
    };

    struct parameters
    {
        amd::design design;

        /// Cores each column runs for this operation. Cores in total are columns * channels.
        std::uint32_t channels{1};

        /// Elements to normalize, split evenly across every core.
        std::uint32_t elements{};

        /// Width of one stored element. bf16 is 2.
        std::uint32_t element_bytes{2};

        /// Whether the design multiplies by a learned weight, which needs a stream of its own.
        bool weighted{true};

        binding input;
        binding weight; ///< Ignored when #weighted is false.
        binding output;

        /**
         * @brief Where each stream is carried, when the design did not take the sequential
         *        arrangement.
         *
         * Left empty, the outlets are derived: two channels per column, filled in order, reads
         * counted separately from writes. That is what the compiler chose for every design whose
         * streams fit the first columns, but its placer is free to put a fifo anywhere the router
         * had room, and for the wider designs it does. Nothing about the operation says where --
         * it is a fact about the compiled design, so it has to travel with the design.
         *
         * #reads covers the cores in order followed by the weight streams; #writes covers the
         * cores in order.
         */
        std::vector<outlet> reads;
        std::vector<outlet> writes;
    };

    explicit rmsnorm(parameters param);

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
    parameters m_param;
};

} // namespace lnpu::nex::amd::programs
