
#include "cmn/layout.hpp"

#include <exception>
#include <sstream>
#include <ranges>
#include <algorithm>

namespace nex
{

layout::layout(shape_container shape) : m_shape(std::move(shape))
{
    compute_strides();
}

layout::layout(std::initializer_list<extent_type> shape) : m_shape(shape)
{
    compute_strides();
}

layout::layout(shape_container shape, stride_container strides, std::size_t offset)
    : m_shape(std::move(shape)),     //
      m_strides(std::move(strides)), //
      m_offset(offset)

{
    if (m_shape.size() != m_strides.size())
    {
        throw std::invalid_argument("Layout: rank mismatch between shape and strides");
    }
}

std::size_t
layout::element_count() const noexcept
{
    std::size_t count = 1;
    for (extent_type e : m_shape)
    {
        if (e < 0)
        {
            return 0;
        }
        count *= static_cast<std::size_t>(e);
    }
    return count;
}

bool
layout::is_contiguous() const noexcept
{
    if (is_dynamic())
    {
        return false;
    }

    extent_type expected = 1;
    for (rank_type i = m_shape.size(); i-- > 0;)
    {
        if (m_shape[i] == 1)
        {
            continue;
        }
        if (m_strides[i] != expected)
        {
            return false;
        }

        expected *= m_shape[i];
    }
    return true;
}

bool
layout::is_dynamic() const noexcept
{
    return std::ranges::any_of(m_shape, [](extent_type v) { return v < 0; });
}

void
layout::compute_strides()
{
    m_strides.resize(m_shape.size());
    extent_type s = 1;
    for (rank_type i = m_shape.size(); i-- > 0;)
    {
        m_strides[i] = s;
        s *= m_shape[i];
    }
}

std::string
layout::to_string() const
{
    auto join = [](std::ostringstream& os, const auto& v) {
        for (std::size_t i = 0; i < v.size(); ++i)
        {
            if (i > 0) os << ", ";
            os << v[i];
        }
    };

    std::ostringstream oss;
    oss << "Layout(shape=[";
    join(oss, m_shape);
    oss << "], strides=[";
    join(oss, m_strides);
    oss << "], offset=" << m_offset << ", elements=" << element_count()
        << ", contiguous=" << (is_contiguous() ? "true" : "false") << ")";
    return oss.str();
}
} // namespace nex
