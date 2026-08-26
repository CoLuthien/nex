
#pragma once

#include <concepts>
#include <iterator>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>

#ifdef __cpp_lib_ranges

namespace lnpu
{

template <bool Const, typename T>
using maybe_const_t = std::conditional_t<Const, const T, T>;

template <std::ranges::input_range V>
requires std::ranges::view<V>
class enumerate_view : public std::ranges::view_interface<enumerate_view<V>>
{
private:
    V base_{};

    template <bool Const>
    class iterator;

    template <bool Const>
    class sentinel;

public:
    enumerate_view() requires std::default_initializable<V>
    = default;

    constexpr explicit enumerate_view(V base) : base_(std::move(base)) {}

    constexpr V base() const& requires std::copy_constructible<V> { return base_; }

    constexpr V base() && { return std::move(base_); }

    // -------------------------------------------------------------------------
    // begin
    // -------------------------------------------------------------------------

    constexpr auto begin()
    {
        return iterator<false>{
            std::ranges::begin(base_),
            0,
        };
    }

    constexpr auto begin() const requires std::ranges::range<const V>
    {
        return iterator<true>{
            std::ranges::begin(base_),
            0,
        };
    }

    // -------------------------------------------------------------------------
    // end
    //
    // common_range이면서 sized_range인 경우 end iterator의 index를 계산할
    // 수 있으므로 iterator를 반환한다.
    //
    // 그 외에는 sentinel을 반환한다.
    // -------------------------------------------------------------------------

    constexpr auto end()
    {
        if constexpr (std::ranges::common_range<V> && std::ranges::sized_range<V>)
        {
            using difference_type = std::ranges::range_difference_t<V>;

            return iterator<false>{
                std::ranges::end(base_),
                static_cast<difference_type>(std::ranges::size(base_)),
            };
        }
        else
        {
            return sentinel<false>{
                std::ranges::end(base_),
            };
        }
    }

    constexpr auto end() const requires std::ranges::range<const V>
    {
        if constexpr (std::ranges::common_range<const V> && std::ranges::sized_range<const V>)
        {
            using difference_type = std::ranges::range_difference_t<const V>;

            return iterator<true>{
                std::ranges::end(base_),
                static_cast<difference_type>(std::ranges::size(base_)),
            };
        }
        else
        {
            return sentinel<true>{
                std::ranges::end(base_),
            };
        }
    }

    // -------------------------------------------------------------------------
    // size
    // -------------------------------------------------------------------------

    constexpr auto size() requires std::ranges::sized_range<V> { return std::ranges::size(base_); }

    constexpr auto size() const requires std::ranges::sized_range<const V>
    {
        return std::ranges::size(base_);
    }

private:
    // -------------------------------------------------------------------------
    // iterator
    // -------------------------------------------------------------------------

    template <bool Const>
    class iterator
    {
    private:
        using Base = maybe_const_t<Const, V>;

        using base_iterator = std::ranges::iterator_t<Base>;

        base_iterator current_{};

        std::ranges::range_difference_t<Base> index_{};

        template <bool>
        friend class iterator;

        template <bool>
        friend class sentinel;

    public:
        using iterator_concept = std::conditional_t<
            std::ranges::random_access_range<Base>,
            std::random_access_iterator_tag,
            std::conditional_t<std::ranges::bidirectional_range<Base>,
                               std::bidirectional_iterator_tag,
                               std::conditional_t<std::ranges::forward_range<Base>,
                                                  std::forward_iterator_tag,
                                                  std::input_iterator_tag>>>;

        // operator*()가 tuple prvalue를 반환하므로 legacy 알고리즘에 대해서는
        // 보수적으로 input iterator로 표시한다.
        using iterator_category = std::input_iterator_tag;

        using difference_type = std::ranges::range_difference_t<Base>;

        using value_type = std::tuple<difference_type, std::ranges::range_value_t<Base>>;

        using reference = std::tuple<difference_type, std::ranges::range_reference_t<Base>>;

        iterator() = default;

        constexpr iterator(base_iterator current, difference_type index)
            : current_(std::move(current)), index_(index)
        {
        }

        // iterator<V> → iterator<const V>
        constexpr iterator(iterator<!Const> other)
            requires Const && std::convertible_to<std::ranges::iterator_t<V>, base_iterator>
            : current_(std::move(other.current_)), index_(other.index_)
        {
        }

        // ---------------------------------------------------------------------
        // Dereference
        // ---------------------------------------------------------------------

        constexpr reference operator*() const
        {
            return {
                index_,
                *current_,
            };
        }

        // ---------------------------------------------------------------------
        // Increment
        // ---------------------------------------------------------------------

        constexpr iterator& operator++()
        {
            ++current_;
            ++index_;

            return *this;
        }

        constexpr void operator++(int) requires(!std::ranges::forward_range<Base>) { ++(*this); }

        constexpr iterator operator++(int) requires std::ranges::forward_range<Base>
        {
            auto previous = *this;
            ++(*this);

            return previous;
        }

        // ---------------------------------------------------------------------
        // Bidirectional operations
        // ---------------------------------------------------------------------

        constexpr iterator& operator--() requires std::ranges::bidirectional_range<Base>
        {
            --current_;
            --index_;

            return *this;
        }

        constexpr iterator operator--(int) requires std::ranges::bidirectional_range<Base>
        {
            auto previous = *this;
            --(*this);

            return previous;
        }

        // ---------------------------------------------------------------------
        // Random-access operations
        // ---------------------------------------------------------------------

        constexpr iterator& operator+=(difference_type offset)
            requires std::ranges::random_access_range<Base>
        {
            current_ += offset;
            index_ += offset;

            return *this;
        }

        constexpr iterator& operator-=(difference_type offset)
            requires std::ranges::random_access_range<Base>
        {
            current_ -= offset;
            index_ -= offset;

            return *this;
        }

        constexpr reference operator[](difference_type offset) const
            requires std::ranges::random_access_range<Base>
        {
            return {
                index_ + offset,
                current_[offset],
            };
        }

        friend constexpr iterator operator+(iterator it, difference_type offset)
            requires std::ranges::random_access_range<Base>
        {
            it += offset;
            return it;
        }

        friend constexpr iterator operator+(difference_type offset, iterator it)
            requires std::ranges::random_access_range<Base>
        {
            it += offset;
            return it;
        }

        friend constexpr iterator operator-(iterator it, difference_type offset)
            requires std::ranges::random_access_range<Base>
        {
            it -= offset;
            return it;
        }

        friend constexpr difference_type operator-(const iterator& lhs, const iterator& rhs)
            requires std::sized_sentinel_for<base_iterator, base_iterator>
        {
            return lhs.current_ - rhs.current_;
        }

        // ---------------------------------------------------------------------
        // Comparisons
        // ---------------------------------------------------------------------

        friend constexpr bool operator==(const iterator& lhs, const iterator& rhs)
            requires std::equality_comparable<base_iterator>
        {
            return lhs.current_ == rhs.current_;
        }

        friend constexpr bool operator<(const iterator& lhs, const iterator& rhs)
            requires std::ranges::random_access_range<Base>
        {
            return lhs.current_ < rhs.current_;
        }

        friend constexpr bool operator>(const iterator& lhs, const iterator& rhs)
            requires std::ranges::random_access_range<Base>
        {
            return rhs < lhs;
        }

        friend constexpr bool operator<=(const iterator& lhs, const iterator& rhs)
            requires std::ranges::random_access_range<Base>
        {
            return !(rhs < lhs);
        }

        friend constexpr bool operator>=(const iterator& lhs, const iterator& rhs)
            requires std::ranges::random_access_range<Base>
        {
            return !(lhs < rhs);
        }

        // underlying element만 swap한다.
        // index는 해당 iterator의 위치 정보이므로 교환하지 않는다.
        friend constexpr void iter_swap(const iterator& lhs, const iterator& rhs) noexcept(
            noexcept(std::ranges::iter_swap(lhs.current_, rhs.current_)))
            requires std::indirectly_swappable<base_iterator>
        {
            std::ranges::iter_swap(lhs.current_, rhs.current_);
        }
    };

    // -------------------------------------------------------------------------
    // sentinel
    // -------------------------------------------------------------------------

    template <bool Const>
    class sentinel
    {
    private:
        using Base = maybe_const_t<Const, V>;

        using base_sentinel = std::ranges::sentinel_t<Base>;

        base_sentinel end_{};

        template <bool>
        friend class sentinel;

    public:
        sentinel() = default;

        constexpr explicit sentinel(base_sentinel end) : end_(std::move(end)) {}

        // sentinel<V> → sentinel<const V>
        constexpr sentinel(sentinel<!Const> other)
            requires Const && std::convertible_to<std::ranges::sentinel_t<V>, base_sentinel>
            : end_(std::move(other.end_))
        {
        }

        friend constexpr bool operator==(const iterator<Const>& it, const sentinel& sent)
        {
            return it.current_ == sent.end_;
        }

        friend constexpr std::ranges::range_difference_t<Base> operator-(const sentinel& sent,
                                                                         const iterator<Const>& it)
            requires std::sized_sentinel_for<base_sentinel, std::ranges::iterator_t<Base>>
        {
            return sent.end_ - it.current_;
        }

        friend constexpr std::ranges::range_difference_t<Base> operator-(const iterator<Const>& it,
                                                                         const sentinel& sent)
            requires std::sized_sentinel_for<base_sentinel, std::ranges::iterator_t<Base>>
        {
            return it.current_ - sent.end_;
        }
    };
};

// -----------------------------------------------------------------------------
// CTAD
// -----------------------------------------------------------------------------

template <typename R>
enumerate_view(R&&) -> enumerate_view<std::views::all_t<R>>;

// -----------------------------------------------------------------------------
// Range adaptor
//
//     container | optai::views::enumerate
// -----------------------------------------------------------------------------

namespace views
{

struct enumerate_fn
{
    template <std::ranges::viewable_range R>
    constexpr auto operator()(R&& range) const
    {
        using View = std::views::all_t<R>;

        return enumerate_view<View>{
            std::views::all(std::forward<R>(range)),
        };
    }

    template <std::ranges::viewable_range R>
    friend constexpr auto operator|(R&& range, const enumerate_fn& self)
    {
        return self(std::forward<R>(range));
    }
};

inline constexpr enumerate_fn enumerate{};

} // namespace views

} // namespace lnpu

#endif // __cpp_lib_ranges
