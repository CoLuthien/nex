#pragma once

#include "amd/program.hpp"

#include <cstdint>
#include <system_error>
#include <vector>

namespace lnpu::nex::amd::programs
{

/**
 * @brief C = A x B, with the weights streamed from DDR because they never fit anywhere else.
 *
 * The design fixes the tile one core chews through and how the cores are laid out; what varies
 * from one call to the next is only how many tiles there are. So the shape reaches the array as
 * two trip counts written into the cores, as descriptor extents, and as how many times a
 * descriptor is re-queued -- never as more commands. A 1536x2048 and a 1536x6144 product emit the
 * same instructions; only the numbers inside them differ. That is what lets one xclbin serve
 * every projection in a model.
 *
 * Two shapes are refused rather than approximated. A matrix with more rows than the array holds
 * at once needs a descriptor per band of rows, which is not what is emitted here.
 *
 * And not every shape a model asks for can be run at all. The descriptor's stride field is 20 bits
 * of 32-bit words, and a B matrix tall enough pushes the stride between its column blocks past
 * that -- Gemma4's 6144x1536 down-projection does, and the compiler refuses the design outright.
 * Such a shape has to be split along K and accumulated, which is a decision for the caller and
 * not something this can paper over, so it is refused here too.
 */
class gemm final : public program
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

        /// How the design lays its cores out. Facts about the xclbin, not about the shape --
        /// they move into gemm::design once describe() reads them out of the descriptor, which
        /// already carries both. Here only so the tree builds while that lands.
        std::uint32_t rows{};
        std::uint32_t first_core_row{2};

        /// One core's tile, as the design was compiled for it.
        std::uint32_t tile_m{};
        std::uint32_t tile_k{};
        std::uint32_t tile_n{};

        /// The product: [m, k] x [k, n].
        std::uint32_t m{};
        std::uint32_t k{};
        std::uint32_t n{};

        std::uint32_t element_bytes{2};

        binding a;
        binding b;
        binding c;

        /// Placement, when the design did not take the sequential arrangement. See rmsnorm.
        std::vector<outlet> reads;
        std::vector<outlet> writes;
    };

    explicit gemm(parameters param);

    std::error_code wire(command_list& sequence) const override;
    std::uint32_t   buffer_descriptors_used() const override;

    /// Times a core walks its tile along K, which is one of the counts the cores are told.
    std::uint32_t k_steps() const;

    /// Tiles of C the array produces in one pass, which is the other count.
    std::uint32_t tile_steps() const;

private:
    parameters m_param;
};

} // namespace lnpu::nex::amd::programs
