
#pragma once

#include "cmn-api.hpp"

#include <cstddef>
#include <initializer_list>
#include <string>
#include <vector>

namespace lnpu
{

class CMN_EXPORT layout
{
public:
    using extent_type      = std::ptrdiff_t;
    using rank_type        = std::size_t;
    using shape_container  = std::vector<extent_type>;
    using stride_container = std::vector<rank_type>;

    friend CMN_EXPORT bool operator==(const layout& lhs, const layout& rhs);

    layout()                             = default;
    layout(const layout&)                = default;
    layout& operator=(const layout&)     = default;
    layout(layout&&) noexcept            = default;
    layout& operator=(layout&&) noexcept = default;

    explicit layout(shape_container shape);
    layout(std::initializer_list<extent_type> shape); // non-explicit: allow `Layout l = {2, 3};`

    // Strides are in ELEMENT units. Where the elements sit within a larger allocation is not
    // stated here: displacement is carried by the buffer, which is what lets one layout describe
    // a region and any view of it alike.
    layout(shape_container shape, stride_container strides);

    [[nodiscard]] rank_type   rank() const noexcept { return m_shape.size(); }
    [[nodiscard]] std::size_t element_count() const noexcept;

    [[nodiscard]] extent_type extent(rank_type dim) const { return m_shape[dim]; }
    [[nodiscard]] extent_type stride(rank_type dim) const { return m_strides[dim]; }

    [[nodiscard]] const shape_container&  shape() const noexcept { return m_shape; }
    [[nodiscard]] const stride_container& strides() const noexcept { return m_strides; }

    // NOTE: no mutable operator[]. Writing a shape element directly would leave the
    // cached strides out of sync (is_contiguous()/element_count() would then lie).
    // Build a new Layout instead, or go through a future reshape()/permute() helper.
    [[nodiscard]] extent_type operator[](rank_type dim) const { return m_shape[dim]; }

    bool        is_contiguous() const noexcept; // C-order (row-major) only
    bool        is_dynamic() const noexcept;
    std::string to_string() const;

private:
    void compute_strides();

    // member variables
    shape_container  m_shape;
    stride_container m_strides;
};

} // namespace lnpu
