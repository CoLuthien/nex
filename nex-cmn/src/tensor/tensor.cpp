
#include "cmn/tensor/tensor.hpp"
#include "cmn/tensor/tensor-view.hpp"
#include "cmn/buffer/mapped-buffer.hpp"

#include <cassert>

namespace nex
{

tensor::tensor(description desc, buffer::shared buffer, passkey)
    : m_description(std::move(desc)),         //
      m_initial_layout(m_description.layout), //
      m_storage(buffer)
{
}

tensor::shared
tensor::create(description description)
{

    assert(description.layout.offset() == 0);
    std::error_code ec{};

    auto allocated = allocate_mapped(description.layout, description.dtype, ec);

    if (ec)
    {
        return nullptr;
    }

    return std::make_shared<tensor>(std::move(description), allocated, passkey{});
}

layout const&
tensor::layout() const
{
    return m_description.layout;
}

std::size_t
tensor::rank() const
{
    return layout().rank();
}

EDataType
tensor::dtype() const
{
    return m_description.dtype;
}

std::size_t
tensor::extent(layout::rank_type location) const
{
    return layout().extent(location);
}

std::size_t
tensor::element_count() const
{
    return layout().element_count();
}

tensor::shared
tensor::create(description description, buffer::shared buffer)
{
    return std::make_shared<tensor>(std::move(description), buffer, passkey{});
}

std::byte*
tensor::data()
{
    return m_storage->ptr();
}

std::byte const*
tensor::data() const
{
    return m_storage->ptr();
}

tensor::view
tensor::slice(coordinate const& begin, coordinate const& end) const
{
    return const_cast<tensor*>(this)->slice(begin, end);
}

tensor::view
tensor::slice(const coordinate& begin, const coordinate& end)
{
    auto& layout = m_description.layout;

    std::size_t const current_rank = layout.rank();

    if (begin.size() != current_rank or end.size() != current_rank)
    {
        throw std::invalid_argument("tensor::slice: rank mismatch");
    }

    std::vector<layout::extent_type> new_shape{};
    new_shape.reserve(current_rank);

    // offset/stride 는 ELEMENT 단위(Layout 계약). byte 로 환산하지 않는다.
    std::size_t extra_offset_elems = 0;

    for (std::size_t i = 0; i < current_rank; ++i)
    {
        if (begin[i] >= end[i] || end[i] > static_cast<std::size_t>(layout.extent(i)))
        {
            throw std::invalid_argument("tensor::slice: Invalid slice indices at dimension " +
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
                        shared_from_this());
}

} // namespace nex
