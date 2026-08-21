

#include "cmn/tensor/tensor-view.hpp"

#include "strided-copy.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace nex
{

tensor::view::view(tensor::description desc, tensor::shared source) //
    : m_description(std::move(desc)),                               //
      m_source(source)
{
}

class layout const&
tensor::view::layout() const
{
    return m_description.layout;
}

std::size_t
tensor::view::rank() const
{
    return m_description.layout.rank();
}

EDataType
tensor::view::dtype() const
{
    return m_description.dtype;
}

std::size_t
tensor::view::extent(layout::extent_type location) const
{
    return static_cast<std::size_t>(
        m_description.layout.extent(static_cast<layout::rank_type>(location)));
}

std::size_t
tensor::view::element_count() const
{
    return m_description.layout.element_count();
}

tensor::description const&
tensor::view::descriptor() const
{
    return m_description;
}

std::byte*
tensor::view::data() noexcept
{
    return m_source->data() + m_description.layout.offset() * in_bytes(m_description.dtype);
}

const std::byte*
tensor::view::data() const noexcept
{
    return m_source->data() + m_description.layout.offset() * in_bytes(m_description.dtype);
}

tensor::shared
tensor::view::contiguous() const
{
    auto const& src_layout = m_description.layout;

    // 뷰의 shape 을 그대로 쓰되 C-order(연속) stride/offset=0 인 새 레이아웃 생성.
    class layout dst_layout{src_layout.shape()};

    auto result = tensor::create(description{.name     = m_description.name,
                                             .dtype    = m_description.dtype,
                                             .layout   = dst_layout,
                                             .encoding = m_description.encoding});

    if (result == nullptr)
    {
        return nullptr;
    }

    // src_base 는 버퍼의 시작점. offset 은 strided_copy 내부에서 layout 을 통해 처리된다.
    detail::strided_copy(m_source->data(), //
                         src_layout,       //
                         result->data(),   //
                         result->layout(), //
                         m_description.dtype);

    return result;
}

tensor::view
tensor::view::slice(const coordinate& begin, const coordinate& end) const
{
    auto const& layout = m_description.layout;

    std::size_t const current_rank = layout.rank();

    if (begin.size() != current_rank or end.size() != current_rank)
    {
        throw std::invalid_argument("tensor::view::slice: rank mismatch");
    }

    std::vector<layout::extent_type> new_shape{};
    new_shape.reserve(current_rank);

    // offset/stride 는 ELEMENT 단위(Layout 계약). byte 로 환산하지 않는다.
    std::size_t extra_offset_elems = 0;

    for (std::size_t i = 0; i < current_rank; ++i)
    {
        if (begin[i] >= end[i] || end[i] > static_cast<std::size_t>(layout.extent(i)))
        {
            throw std::invalid_argument("tensor::view::slice: Invalid slice indices at dimension " +
                                        std::to_string(i) + ": begin=" + std::to_string(begin[i]) +
                                        ", end=" + std::to_string(end[i]) +
                                        ", extent=" + std::to_string(layout.extent(i)));
        }

        new_shape.push_back(end[i] - begin[i]);
        extra_offset_elems += begin[i] * layout.stride(i);
    }

    class layout new_layout{new_shape, layout.strides(), layout.offset() + extra_offset_elems};

    return tensor::view(description{.name     = m_description.name,
                                    .dtype    = m_description.dtype,
                                    .layout   = new_layout,
                                    .encoding = m_description.encoding},
                        m_source);
}

} // namespace nex
