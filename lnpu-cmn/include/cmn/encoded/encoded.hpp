
#pragma once

#include "cmn-api.hpp"
#include "cmn/layout.hpp"
#include "cmn/numeric-types.hpp"
#include "cmn/buffer/buffer-interface.hpp"

#include <cstdint>
#include <memory>
#include <optional>
#include <span>
#include <string>

namespace lnpu
{

class CMN_EXPORT encoded final
{

public:
    using shared      = std::shared_ptr<encoded>;
    using unique      = std::unique_ptr<encoded>;
    using layout_type = lnpu::layout;

    struct quantization
    {
        /**
         * @brief Dequantization scale. Required whenever a quantization is present (non-null).
         *        A scalar tensor denotes per-tensor quantization; a 1-D tensor denotes
         *        per-channel quantization along #axis.
         */
        encoded::shared scale;

        /**
         * @brief Zero point. May be null, denoting symmetric quantization (zero_point == 0).
         *        When present, its shape matches #scale.
         */
        encoded::shared zero_point;

        /**
         * @brief Channel axis for per-channel quantization; empty for per-tensor.
         *        Stored as a non-negative index (the importer normalizes negative ONNX axes).
         */
        std::optional<std::size_t> axis;

        /**
         * @brief Width in bits of the code range this mapping is defined over.
         *
         * Not the width the codes are STORED in. The two agree once a region actually holds
         * codes, but they part company on fake-quant metadata: a float region describing an
         * 8-bit mapping has an f32 #encoding and a bitwidth of 8, and the bitwidth is then the
         * only thing left saying what the values were quantized for. A reader deciding between
         * kernels before the weights are converted has nothing else to ask.
         *
         * Zero is not a bitwidth. validate() refuses it rather than let a mapping nobody filled
         * in pass for an 8-bit one.
         */
        std::uint8_t bitwidth{};
    };

    struct format
    {
        std::string                 name;
        EDataType                   encoding{EDataType::invalid};
        lnpu::layout                layout;
        std::optional<quantization> quant;
    };

    class slice;

private:
    buffer::shared     m_buffer;
    format             m_format;
    lnpu::layout const m_initial_layout;

    /**
     * @brief The quantization parameters as the region was built with them, kept for the same
     *        reason as #m_initial_layout.
     *
     * reshape() derives the per-channel parameters of the new shape from THESE, never from the ones
     * it derived last time. A shape may shrink and grow back within the extents the buffer was
     * sized for, and parameters narrowed in place would leave the outer channels with nothing to be
     * restored from; every extent a reshape can reach is a prefix of these, since no dimension may
     * outgrow the one it was built with.
     */
    std::optional<quantization> const m_initial_quant;

public:
    explicit encoded(format&& format, buffer::shared buffer);
    ~encoded() = default;

    static encoded::shared create(format format, buffer::shared buffer);

    static void copy(encoded const& from, encoded& to);
    static void copy(encoded const& from, slice& to);

    static void copy(slice const& from, encoded& to);
    static void copy(slice const& from, slice& to);

    // delete copy/move
    encoded(const encoded&)                = delete;
    encoded& operator=(const encoded&)     = delete;
    encoded(encoded&&) noexcept            = delete;
    encoded& operator=(encoded&&) noexcept = delete;

    std::byte*       ptr();
    std::byte const* ptr() const;

    template <number T>
    T* ptr()
    {
        return reinterpret_cast<T*>(ptr());
    }

    template <number T>
    T const* ptr() const
    {
        return reinterpret_cast<T const*>(ptr());
    }

    /**
     * @brief Element iterators over the region as a dense C-order array of T, same contract as
     *        ptr<T>(): T must match what dtype() stores.
     */
    template <number T>
    T* begin()
    {
        return ptr<T>();
    }

    template <number T>
    T* end()
    {
        return ptr<T>() + n_element();
    }

    template <number T>
    T const* begin() const
    {
        return ptr<T>();
    }

    template <number T>
    T const* end() const
    {
        return ptr<T>() + n_element();
    }

    /**
     * @brief The elements as one span, ready for range adaptors.
     */
    template <number T>
    std::span<T> values()
    {
        return {begin<T>(), end<T>()};
    }

    template <number T>
    std::span<T const> values() const
    {
        return {begin<T>(), end<T>()};
    }

    template <number T>
    encoded::shared cast() const;

    template <number T>
    void cast(encoded::shared buffer) const;

    slice take(layout_type::shape_container const& from,
               layout_type::shape_container const& to) const;
    slice take(layout_type::shape_container const& to) const;
    slice as_slice() const;

    /**
     * @brief Gives the region a new shape within the one it was built with, keeping every element
     *        both shapes have room for at the same logical coordinate.
     *
     * Only the SHAPE of @p layout is read: the region is handed back densely laid out (C-order,
     * no offset), and @p compaction is what moves the elements into it. Pass false when the
     * contents are about to be overwritten anyway -- nothing is moved then, and what the bytes
     * mean under the new shape is the caller's business.
     *
     * A per-channel quantization follows the new shape: the region is left with the parameters of
     * the channels the shape has room for, which is a prefix of the ones it was built with (see
     * #m_initial_quant) -- so growing the axis back restores them exactly. Parameters that do not
     * describe the extent the region was built with are passed on untouched, with a warning.
     *
     * @throws std::invalid_argument on a rank mismatch, a negative extent, or an encoding with no
     *         fixed element size.
     * @throws std::out_of_range when a dimension grows past the extent the region was built with,
     *         or the request outgrows the buffer.
     */
    void reshape(lnpu::layout layout, bool compaction = true);

    buffer::shared source() const;

    layout_type::rank_type rank() const;
    std::size_t            n_element() const;
    std::size_t            n_bytes() const;
    EDataType              dtype() const;

    layout_type::shape_container const&  shape() const;
    layout_type::stride_container const& stride() const;

    layout_type::extent_type shape(layout_type::rank_type dim) const;
    layout_type::extent_type stride(layout_type::rank_type dim) const;

    layout_type   layout() const;
    format const& fmt() const;

    /**
     * @brief Whether the stored values have to be dequantized before they mean anything.
     *
     * Carrying a quantization is not the same as being quantized: on a float or half encoding the
     * parameters are fake-quant metadata, the values are already real, and applying the scale
     * again would silently halve them. This answers the question a reader actually has, so it is
     * true only when parameters are paired with an encoding that stores codes. The metadata
     * itself, live or not, is always readable through fmt().
     */
    bool is_quantized() const;

    /**
     * @brief The dequantization parameters, or an empty span when there are none to apply.
     *
     * Empty exactly when is_quantized() is false. One entry for a per-tensor quantization, one
     * per channel otherwise; zero_point() is additionally empty for a symmetric quantization,
     * where every entry would be zero. Both point into the parameter regions and stay valid as
     * long as this one does.
     */
    std::span<float const>        scale() const;
    std::span<std::int32_t const> zero_point() const;

private:
    void compact_inplace(layout_type::shape_container const& from,
                         layout_type::shape_container const& to);
};

} // namespace lnpu
