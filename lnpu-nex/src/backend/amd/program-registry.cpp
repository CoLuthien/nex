#include "program-registry.hpp"

#include "programs/gemm.hpp"
#include "programs/rmsnorm.hpp"

#include <string_view>
#include <unordered_map>

namespace lnpu::nex::amd
{
namespace
{

/**
 * @brief Adapts one program's lower() to the one signature the table stores.
 *
 * This is the whole of the type erasure. A pointer to Program::lower cannot be stored directly --
 * it returns shared_ptr<Program>, and a function pointer's return type does not convert the way
 * the pointee does -- so the conversion happens here, once, for every operator.
 */
template <typename Program>
program::shared
lower_as(descriptor const& metadata, layer_description const& layer, std::error_code& ec)
{
    return Program::lower(metadata, layer, ec);
}

/**
 * @brief Every operator this build can run, by the op_type an onnx node states.
 *
 * Two entries may name one program: a Gemm and a MatMul are the same product to the array, and
 * what differs between them -- an alpha, a bias input -- is read inside gemm::lower(), which is
 * where the knowledge of what a Gemm means already lives. That is normalization, not dispatch,
 * so it does not belong up here.
 *
 * design_op is taken from the program's own kOp rather than written out again, so the name the
 * table looks an xclbin up by and the name that program's describe() insists on cannot drift
 * apart.
 *
 * The key is op_type alone for now. Dispatch properly keys on (domain, op_type, opset) -- the
 * frontend's schema_registry already does -- and this will have to when a custom domain first
 * spells an operator the standard domain also spells; layer_description states only op_type
 * today, so there is nothing yet to disagree about.
 */
std::unordered_map<std::string_view, lowering> const&
table()
{
    static std::unordered_map<std::string_view, lowering> const lowerings{
        {"Gemm", {programs::gemm::kOp, &lower_as<programs::gemm>}},
        {"MatMul", {programs::gemm::kOp, &lower_as<programs::gemm>}},
        {"RMSNormalization", {programs::rmsnorm::kOp, &lower_as<programs::rmsnorm>}},
    };

    return lowerings;
}

} // namespace

lowering const*
find_lowering(std::string_view op_type)
{
    auto const& lowerings = table();

    auto const found = lowerings.find(op_type);

    return found == lowerings.end() ? nullptr : &found->second;
}

} // namespace lnpu::nex::amd
