#pragma once

#include "amd/program.hpp"

#include "nex/frontend/layer-description.hpp"

#include <cstdint>
#include <memory>
#include <string_view>
#include <system_error>

namespace lnpu::nex::amd
{
class descriptor;
}

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
    /// The descriptor's "op". A design that says anything else was baked for another program.
    static constexpr std::string_view kOp{"gemm"};

    /**
     * @brief What the xclbin fixed for this operator.
     *
     * No shape is in here, which is the point: the cores read their trip counts out of their own
     * data memory and this design serves every projection in a model. Four (K, N) combinations
     * bake to a byte-identical array.
     */
    struct design
    {
        amd::design common;

        /// Core rows a column puts to work, counted from #first_core_row.
        std::uint32_t rows{};

        /// The first compute row. Row 0 is the shim and row 1 the mem tile, so cores start at 2.
        std::uint32_t first_core_row{2};

        /// One core's tile, as the design was compiled for it.
        std::uint32_t tile_m{};
        std::uint32_t tile_k{};
        std::uint32_t tile_n{};

        std::uint32_t element_bytes{};
    };

    /**
     * @throws std::runtime_error when the descriptor is for another operator, or is missing a
     *         field this needs.
     */
    static design describe(descriptor const& metadata);

    struct parameters
    {
        /// The product: [m, k] x [k, n].
        std::uint32_t m{};
        std::uint32_t k{};
        std::uint32_t n{};

        binding a;
        binding b;
        binding c;

        /**
         * @brief Wait for every transfer the run started, not only for the result.
         *
         * Off, the run is finished when C has been written, which is what aiecc emits and what
         * IRON needs: its flow bakes one xclbin per shape and runs it, so no second shape ever
         * follows on the same array and nothing has to be left tidy.
         *
         * We do follow one shape with another, and that is where it shows. Running four shapes
         * against one hardware context, the two whose K differed from the previous run returned
         * wrong numbers and then timed out on every repeat, while the one that only changed N was
         * fine -- and each of the four was correct when given a context of its own. What the A
         * and B streams carry scales with K, and they are the transfers nothing here waits for.
         *
         * On, every stream this run queued announces itself and is waited on: twelve more waits
         * on the input side. The stream is then no longer the one aiecc produces, which is the
         * point rather than a problem -- but it is also why the reference streams shipped beside
         * the artifacts only match with this off.
         */
        bool wait_for_inputs{false};
    };

    /**
     * @brief Builds the program that runs @p layer on the design @p metadata describes.
     *
     * The shape is read off the layer's inputs and the argument slots off the descriptor, so this
     * is the one place that knows what a Gemm node means -- and it is beside the program that has
     * to run it, not in a table that would need to know every operator to hold their fields.
     *
     * Every refusal is an @p ec rather than a throw. A graph naming a shape this design cannot
     * run is an ordinary answer for an orchestrator walking a model, not a programming error, and
     * it has to be able to say which layer it was without unwinding.
     *
     * @return nullptr with @p ec set when the layer cannot be run on this design.
     */
    static std::shared_ptr<gemm> lower(descriptor const&        metadata,
                                       layer_description const& layer,
                                       std::error_code&         ec);

    gemm(design fixed, parameters param);

    std::error_code wire(command_list& sequence) const override;
    std::uint32_t   buffer_descriptors_used() const override;

    /// Times a core walks its tile along K, which is one of the counts the cores are told.
    std::uint32_t k_steps() const;

    /// Tiles of C the array produces in one pass, which is the other count.
    std::uint32_t tile_steps() const;

private:
    design     m_design;
    parameters m_param;
};

} // namespace lnpu::nex::amd::programs
