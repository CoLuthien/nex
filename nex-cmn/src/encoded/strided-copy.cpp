#include "strided-copy.hpp"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <string>

namespace nex::detail
{
namespace
{

constexpr std::size_t kMaxDims = 8;

/**
 * @brief Whether two scale / zero_point operands denote the same numbers. Cheap pointer equality
 *        first; a full comparison only when they are distinct objects.
 */
bool
same_parameters(encoded::shared const& lhs, encoded::shared const& rhs)
{
    if (lhs == rhs) return true;
    if (not lhs or not rhs) return false;

    if (lhs->dtype() != rhs->dtype() or lhs->shape() != rhs->shape()) return false;

    return std::memcmp(lhs->ptr(), rhs->ptr(), lhs->n_bytes()) == 0;
}

bool
same_quantization(encoded::quantization const& lhs, encoded::quantization const& rhs)
{
    return lhs.axis == rhs.axis and same_parameters(lhs.scale, rhs.scale) and
           same_parameters(lhs.zero_point, rhs.zero_point);
}

void
check_formats(encoded::format const& src, encoded::format const& dst)
{
    if (src.encoding != dst.encoding)
    {
        throw std::invalid_argument(
            "strided_copy: encoding mismatch (src=" + to_string(src.encoding) +
            ", dst=" + to_string(dst.encoding) + ")");
    }

    if (src.quant.has_value() and dst.quant.has_value())
    {
        if (not same_quantization(*src.quant, *dst.quant))
        {
            throw std::invalid_argument(
                "strided_copy: quantization mismatch between '" + src.name + "' and '" + dst.name +
                "'; a raw copy would reinterpret the values. Use cast<T>() to requantize");
        }
    }
    else if (src.quant.has_value() != dst.quant.has_value())
    {
        spdlog::warn("[strided_copy] copying between '{}' and '{}' where only one side carries a "
                     "quantization; the raw codes are moved as-is",
                     src.name,
                     dst.name);
    }
}

template <typename Src, typename Dst>
void
copy_impl(Src const& src, Dst& dst)
{
    check_formats(src.fmt(), dst.fmt());

    strided_copy_bytes(src.ptr(), src.layout(), dst.ptr(), dst.layout(), src.dtype());
}

} // namespace

void
strided_copy_bytes(std::byte const* src_base,
                   layout const&    src_layout,
                   std::byte*       dst_base,
                   layout const&    dst_layout,
                   EDataType        dtype)
{
    auto const rank = src_layout.rank();

    if (rank != dst_layout.rank())
    {
        throw std::invalid_argument("strided_copy: rank mismatch (src=" + std::to_string(rank) +
                                    ", dst=" + std::to_string(dst_layout.rank()) + ")");
    }

    for (std::size_t i = 0; i < rank; ++i)
    {
        if (src_layout.extent(i) != dst_layout.extent(i))
        {
            throw std::invalid_argument("strided_copy: shape mismatch at dim " + std::to_string(i) +
                                        " (src=" + std::to_string(src_layout.extent(i)) +
                                        ", dst=" + std::to_string(dst_layout.extent(i)) + ")");
        }
    }

    auto const n_element = src_layout.element_count();
    if (n_element == 0) return;

    if (not has_fixed_element_size(dtype))
    {
        throw std::invalid_argument("strided_copy: dtype " + to_string(dtype) +
                                    " has no fixed element size");
    }
    auto const elem_size = in_bytes(dtype);

    if (src_base == nullptr or dst_base == nullptr)
    {
        throw std::invalid_argument("strided_copy: null buffer");
    }

    if (src_layout.is_contiguous() and dst_layout.is_contiguous())
    {
        std::memcpy(dst_base, src_base, n_element * elem_size);
        return;
    }

    if (rank > kMaxDims)
    {
        throw std::invalid_argument("strided_copy: rank " + std::to_string(rank) +
                                    " exceeds the limit of " + std::to_string(kMaxDims));
    }

    auto const& src_strides = src_layout.strides();
    auto const& dst_strides = dst_layout.strides();

    // Merge each dimension into the run inside it while the outer stride equals
    // (innermost stride of the run) * (extent merged so far) on BOTH sides -- that describes the
    // same walk as one longer dimension.
    std::size_t merged_extent[kMaxDims];
    std::size_t merged_src[kMaxDims]; // bytes
    std::size_t merged_dst[kMaxDims]; // bytes
    std::size_t merged_rank = 0;

    for (std::size_t i = rank; i-- > 0;)
    {
        auto       extent  = static_cast<std::size_t>(src_layout.extent(i));
        auto const run_src = static_cast<std::size_t>(src_strides[i]) * elem_size;
        auto const run_dst = static_cast<std::size_t>(dst_strides[i]) * elem_size;

        while (i > 0)
        {
            auto const outer = i - 1;
            if (static_cast<std::size_t>(src_strides[outer]) * elem_size != run_src * extent or
                static_cast<std::size_t>(dst_strides[outer]) * elem_size != run_dst * extent)
            {
                break;
            }

            extent *= static_cast<std::size_t>(src_layout.extent(outer));
            --i;
        }

        merged_extent[merged_rank] = extent;
        merged_src[merged_rank]    = run_src;
        merged_dst[merged_rank]    = run_dst;
        ++merged_rank;
    }

    // collected innermost-first; flip to outermost-first
    std::reverse(merged_extent, merged_extent + merged_rank);
    std::reverse(merged_src, merged_src + merged_rank);
    std::reverse(merged_dst, merged_dst + merged_rank);

    if (merged_rank == 0) // rank-0 (scalar) layout
    {
        std::memcpy(dst_base, src_base, elem_size);
        return;
    }

    auto const inner_extent = merged_extent[merged_rank - 1];
    auto const inner_src    = merged_src[merged_rank - 1];
    auto const inner_dst    = merged_dst[merged_rank - 1];
    auto const inner_dense  = (inner_src == elem_size and inner_dst == elem_size);
    auto const inner_bytes  = inner_extent * elem_size;
    auto const outer_rank   = merged_rank - 1;

    auto copy_inner = [&](std::byte* d, std::byte const* s) {
        if (inner_dense)
        {
            std::memcpy(d, s, inner_bytes);
            return;
        }
        for (std::size_t k = 0; k < inner_extent; ++k)
        {
            std::memcpy(d + k * inner_dst, s + k * inner_src, elem_size);
        }
    };

    if (outer_rank == 0)
    {
        copy_inner(dst_base, src_base);
        return;
    }

    // outer_rank <= kMaxDims - 1, so the odometer fits on the stack
    std::size_t position[kMaxDims]{};
    std::size_t total_iters = 1;
    for (std::size_t d = 0; d < outer_rank; ++d)
    {
        total_iters *= merged_extent[d];
    }

    std::size_t src_offset = 0;
    std::size_t dst_offset = 0;

    for (std::size_t iter = 0; iter < total_iters; ++iter)
    {
        copy_inner(dst_base + dst_offset, src_base + src_offset);

        for (std::size_t d = outer_rank; d-- > 0;)
        {
            src_offset += merged_src[d];
            dst_offset += merged_dst[d];

            if (++position[d] < merged_extent[d]) break;

            src_offset -= merged_extent[d] * merged_src[d];
            dst_offset -= merged_extent[d] * merged_dst[d];
            position[d] = 0;
        }
    }
}

void
strided_copy(encoded const& src, encoded& dst)
{
    copy_impl(src, dst);
}

void
strided_copy(encoded const& src, encoded::slice& dst)
{
    copy_impl(src, dst);
}

void
strided_copy(encoded::slice const& src, encoded& dst)
{
    copy_impl(src, dst);
}

void
strided_copy(encoded::slice const& src, encoded::slice& dst)
{
    copy_impl(src, dst);
}

} // namespace nex::detail
