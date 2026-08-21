
#include "strided-copy.hpp"

#include "cmn/layout.hpp"
#include "cmn/numeric-types.hpp"

#include <utility>
#include <cstring>
#include <vector>
#include <algorithm>
#include <stdexcept>

namespace nex::detail
{
void
strided_copy(const std::byte* src_base,
             layout const&    src_layout,
             std::byte*       dst_base,
             layout const&    dst_layout,
             EDataType        dtype)
{
    const std::size_t rank = src_layout.rank();

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

    const std::size_t elem_count = src_layout.element_count();
    if (elem_count == 0) return;

    const std::size_t elem_size = in_bytes(dtype);
    if (elem_size == 0)
    {
        throw std::invalid_argument("strided_copy: invalid dtype (in_bytes == 0)");
    }
    if (src_base == nullptr || dst_base == nullptr)
    {
        throw std::invalid_argument("strided_copy: null buffer");
    }

    const std::byte* src = src_base + src_layout.offset() * in_bytes(dtype);
    std::byte*       dst = dst_base + dst_layout.offset() * in_bytes(dtype);

    // Fast path: 둘 다 contiguous
    if (src_layout.is_contiguous() && dst_layout.is_contiguous())
    {
        std::memcpy(dst, src, elem_count * elem_size);
        return;
    }

    constexpr std::size_t kMaxDims = 8;
    if (rank > kMaxDims)
    {
        throw std::invalid_argument("strided_copy: rank " + std::to_string(rank) +
                                    " exceeds kMaxDims " + std::to_string(kMaxDims));
    }

    const auto& src_strides = src_layout.strides();
    const auto& dst_strides = dst_layout.strides();

    // Stride 압축: src/dst 각각의 segment 최내 stride 기준으로 병합
    std::size_t comp_extents[kMaxDims];
    std::size_t comp_src_strides[kMaxDims]; // bytes
    std::size_t comp_dst_strides[kMaxDims]; // bytes
    std::size_t comp_rank = 0;

    std::size_t i = rank;
    while (i > 0)
    {
        --i;

        std::size_t       merged_extent = src_layout.extent(i);
        const std::size_t seg_src       = src_strides[i] * elem_size; // 이 segment의 최내 stride
        const std::size_t seg_dst       = dst_strides[i] * elem_size;

        while (i > 0)
        {
            const std::size_t p = i - 1;
            // 병합 조건: 바깥 차원 stride == (segment 최내 stride) * (지금까지 병합된 extent)
            if (src_strides[p] * elem_size == seg_src * merged_extent &&
                dst_strides[p] * elem_size == seg_dst * merged_extent)
            {
                merged_extent *= src_layout.extent(p);
                --i;
            }
            else
            {
                break;
            }
        }

        comp_extents[comp_rank]     = merged_extent;
        comp_src_strides[comp_rank] = seg_src;
        comp_dst_strides[comp_rank] = seg_dst;
        ++comp_rank;
    }

    // 역순 정렬 (outermost first)
    for (std::size_t j = 0; j < comp_rank / 2; ++j)
    {
        std::swap(comp_extents[j], comp_extents[comp_rank - 1 - j]);
        std::swap(comp_src_strides[j], comp_src_strides[comp_rank - 1 - j]);
        std::swap(comp_dst_strides[j], comp_dst_strides[comp_rank - 1 - j]);
    }

    if (comp_rank == 0) // rank 0 방어
    {
        std::memcpy(dst, src, elem_size);
        return;
    }

    const std::size_t inner_extent     = comp_extents[comp_rank - 1];
    const std::size_t inner_src_stride = comp_src_strides[comp_rank - 1];
    const std::size_t inner_dst_stride = comp_dst_strides[comp_rank - 1];
    const bool inner_contiguous = (inner_src_stride == elem_size && inner_dst_stride == elem_size);
    const std::size_t inner_copy_bytes = inner_extent * elem_size;
    const std::size_t outer_dims       = comp_rank - 1;

    // 최내차원: 연속이면 block memcpy, 아니면 원소별 복사
    auto copy_inner = [&](std::byte* d, const std::byte* s) {
        if (inner_contiguous)
        {
            std::memcpy(d, s, inner_copy_bytes);
        }
        else
        {
            for (std::size_t k = 0; k < inner_extent; ++k)
            {
                std::memcpy(d + k * inner_dst_stride, s + k * inner_src_stride, elem_size);
            }
        }
    };

    if (outer_dims == 0)
    {
        copy_inner(dst, src);
        return;
    }

    // outer_dims <= kMaxDims - 1 이므로 stack 만으로 충분 (heap 경로 제거)
    std::size_t pos[kMaxDims];
    std::size_t extents[kMaxDims];
    std::size_t total_iters = 1;
    for (std::size_t d = 0; d < outer_dims; ++d)
    {
        pos[d]     = 0;
        extents[d] = comp_extents[d];
        total_iters *= extents[d];
    }

    std::size_t src_offset = 0;
    std::size_t dst_offset = 0;

    for (std::size_t iter = 0; iter < total_iters; ++iter)
    {
        copy_inner(dst + dst_offset, src + src_offset);

        for (std::size_t d = outer_dims; d-- > 0;)
        {
            src_offset += comp_src_strides[d];
            dst_offset += comp_dst_strides[d];

            if (++pos[d] < extents[d]) break;

            src_offset -= extents[d] * comp_src_strides[d];
            dst_offset -= extents[d] * comp_dst_strides[d];
            pos[d] = 0;
        }
    }
}

} // namespace nex::detail
