#pragma once

#include "cmn-api.hpp"
#include "cmn/encoded/encoded.hpp"
#include "cmn/encoded/encoded-slice.hpp"
#include "cmn/buffer/mapped-buffer.hpp"
#include "cmn/floats/f16.hpp"
#include "cmn/floats/bf16.hpp"
#include "cmn/numeric-types.hpp"
#include "cmn/dispatch.hpp"

#include <algorithm>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <limits>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <system_error>
#include <type_traits>

/*
 * Value pipeline of a cast, symmetric in both directions:
 *
 *     src element --to_real--> real --[dequantize]--> real
 *                             real --[quantize]----> real --from_real--> dst element
 *
 *   - dequantize runs when the SOURCE carries a quantization and its element type can hold one
 *     (integers and uf8/uf16/uf32). A quantization sitting on a float/f16/bf16 source is
 *     fake-quant metadata -- the stored values are already real -- so it is ignored.
 *   - quantize runs under the mirror-image condition on the DESTINATION.
 *   - both  -> requantize, neither -> plain format conversion (no double detour where it would
 *     be lossy, see convert()).
 *
 * Quantization parameters always come from the encoded that owns them, so the only way to ask
 * for the quantize direction is the in-place overload cast<T>(encoded::shared): the caller
 * allocates a destination whose format already carries scale/zero_point/axis. The out-of-place
 * cast<T>() has nowhere to get them from and rejects the fixed-point types at compile time.
 */
namespace nex
{
namespace detail
{

/*
 * Reported out-of-line so that this header -- and every consumer of it -- stays free of the
 * logging backend. The definitions live in src/encoded/cast-diagnostics.cpp.
 */
CMN_EXPORT void log_quantization_ignored(std::string const& name, std::string const& type);
CMN_EXPORT void log_quantization_missing(std::string const& name);
CMN_EXPORT void log_allocation_failed(std::string const& name, std::string const& message);

/**
 * @brief Raw in-memory representation of a logical element type.
 */
// clang-format off
template <typename T> struct storage { using type = T; };
template <> struct storage<f16>      { using type = std::uint16_t; };
template <> struct storage<bf16>     { using type = std::uint16_t; };
template <> struct storage<uf8>      { using type = std::uint8_t; };
template <> struct storage<uf16>     { using type = std::uint16_t; };
template <> struct storage<uf32>     { using type = std::uint32_t; };
// clang-format on

template <typename T>
using storage_t = typename storage<T>::type;

/**
 * @brief Whether an affine (scale, zero_point) quantization can be carried by @p T. True for the
 *        integer types and the unsigned fixed-point types; false for real types, whose stored
 *        values are the dequantized ones already.
 */
template <typename T>
inline constexpr bool is_quantizable = std::integral<T> or is_fixed_point_v<T>;

/**
 * @brief Decode one stored element into a real value.
 */
template <number T>
inline double
to_real(T v) noexcept
{
    if constexpr (is_half_precision_v<T>)
    {
        return static_cast<double>(as_f32(v));
    }
    else if constexpr (is_fixed_point_v<T>)
    {
        return static_cast<double>(v.value);
    }
    else
    {
        return static_cast<double>(v);
    }
}

/**
 * @brief Encode a real value into one stored element. Integer and fixed-point destinations
 *        truncate toward zero and do NOT saturate -- quantize() clamps before calling here.
 */
template <number T>
inline T
from_real(double v) noexcept
{
    if constexpr (std::is_same_v<T, f16>)
    {
        return as_f16(static_cast<float>(v));
    }
    else if constexpr (std::is_same_v<T, bf16>)
    {
        return as_bf16(static_cast<float>(v));
    }
    else if constexpr (is_fixed_point_v<T>)
    {
        return T{static_cast<storage_t<T>>(v)};
    }
    else
    {
        return static_cast<T>(v);
    }
}

/**
 * @brief Unquantized element conversion. Integer-to-integer stays in the integer domain instead
 *        of routing through double, which would lose the low bits of large 64-bit values.
 */
template <number Dst, number Src>
inline Dst
convert(Src v) noexcept
{
    if constexpr (std::is_same_v<Src, Dst>)
    {
        return v;
    }
    else if constexpr (std::integral<Src> and std::integral<Dst>)
    {
        return static_cast<Dst>(v);
    }
    else
    {
        return from_real<Dst>(to_real<Src>(v));
    }
}

/**
 * @brief Affine quantize, clamped to the destination's storage range. Mirrors the rounding of
 *        the ONNX QuantizeLinear reference (round-half-to-even via nearbyint).
 */
template <number T>
inline T
quantize(double real, double scale, double zero_point) noexcept
{
    using S = storage_t<T>;

    auto q = std::nearbyint(real / scale) + zero_point;
    q      = std::clamp(q,
                        static_cast<double>(std::numeric_limits<S>::lowest()),
                        static_cast<double>(std::numeric_limits<S>::max()));
    return from_real<T>(q);
}

/**
 * @brief Quantization parameters paired with the grouping they apply along: one (scale,
 *        zero_point) pair per group. Per-tensor quantization degenerates to a single group.
 *
 * The parameters are not copied. scale() / zero_point() hand out the stored f32 and i32 regions
 * directly, which is the whole point of normalizing them at construction, and they stay valid as
 * long as the region being cast does -- longer than this table.
 */
struct affine_table
{
    std::span<float const>        scale;
    std::span<std::int32_t const> zero_point; // empty for a symmetric quantization
    std::size_t group_stride = 1;             // elements between two consecutive group changes
    std::size_t groups       = 1;

    [[nodiscard]] std::size_t group_of(std::size_t index) const noexcept
    {
        return groups == 1 ? 0 : (index / group_stride) % groups;
    }

    [[nodiscard]] double scale_at(std::size_t group) const noexcept
    {
        return static_cast<double>(scale[group]);
    }

    [[nodiscard]] double zero_point_at(std::size_t group) const noexcept
    {
        return zero_point.empty() ? 0.0 : static_cast<double>(zero_point[group]);
    }
};

/**
 * @brief Pairs the parameters @p region stores with its layout, which must be contiguous --
 *        group_stride is the product of the extents trailing the channel axis (its C-order
 *        stride).
 *
 * Only the pairing is checked here. That the parameters are STORED in a form these spans can hand
 * out is settled once, when the region is built; that their count describes this layout is not, so
 * it is checked where it is about to be relied on. take() keeps the two in step as it narrows a
 * channel axis, but an importer or a hand-built format can still disagree.
 *
 * @pre region.is_quantized(), so the scale span is non-empty.
 */
template <typename Region>
inline affine_table
resolve_affine(Region const& region)
{
    auto const& fmt = region.fmt();

    affine_table table{.scale = region.scale(), .zero_point = region.zero_point()};

    if (std::any_of(table.scale.begin(), table.scale.end(), [](float s) { //
            return s == 0.0F or not std::isfinite(s);
        }))
    {
        throw std::invalid_argument("quantization scale must be finite and non-zero");
    }

    auto const& of = fmt.layout;

    if (fmt.quant->axis)
    {
        auto const axis = *fmt.quant->axis;
        if (axis >= of.rank())
        {
            throw std::out_of_range("quantization axis is out of range for the encoded layout");
        }

        table.groups       = static_cast<std::size_t>(of.extent(axis));
        table.group_stride = 1;
        for (auto dim = axis + 1; dim < of.rank(); ++dim)
        {
            table.group_stride *= static_cast<std::size_t>(of.extent(dim));
        }

        if (table.scale.size() != table.groups)
        {
            throw std::invalid_argument("per-channel scale count must match the axis extent");
        }
    }
    else if (table.scale.size() != 1)
    {
        throw std::invalid_argument("per-tensor quantization requires a scalar scale");
    }

    return table;
}

/**
 * @brief Resolves the quantization @p region asks for, or nullopt when there is none to apply --
 *        either the region carries no live quantization, or @p T cannot hold one.
 *
 * is_quantized() answers both questions at once: it is false exactly when the encoding does not
 * store codes, and @p T is that encoding's type. The compile-time branch survives only to say so
 * out loud, since a caller who asked for a real type usually wants to know its scale was skipped.
 */
template <typename T, typename Region>
inline std::optional<affine_table>
affine_of(Region const& region)
{
    if constexpr (not is_quantizable<T>)
    {
        if (region.fmt().quant)
        {
            log_quantization_ignored(region.fmt().name, to_string(type_encoded_v<T>));
        }
        return std::nullopt;
    }
    else
    {
        if (not region.is_quantized())
        {
            return std::nullopt;
        }
        return resolve_affine(region);
    }
}

template <typename DstT>
struct cast_to
{
    template <typename SrcT>
    struct from
    {
        // Src is either encoded or encoded::slice -- both expose ptr/n_element/fmt.
        template <typename Src>
        static void run(Src const& src, encoded& dst)
        {
            auto const  n   = static_cast<std::size_t>(src.n_element());
            auto const* in  = src.template ptr<SrcT>();
            auto*       out = dst.ptr<DstT>();

            auto const dequant = affine_of<SrcT>(src);
            auto const requant = affine_of<DstT>(dst);

            if (dequant or requant)
            {
                for (std::size_t i = 0; i < n; ++i)
                {
                    auto real = to_real<SrcT>(in[i]);

                    if (dequant)
                    {
                        auto const g = dequant->group_of(i);
                        real         = (real - dequant->zero_point_at(g)) * dequant->scale_at(g);
                    }

                    if constexpr (is_quantizable<DstT>)
                    {
                        if (requant)
                        {
                            auto const g = requant->group_of(i);

                            out[i] = quantize<DstT>(real, //
                                                    requant->scale_at(g),
                                                    requant->zero_point_at(g));
                            continue;
                        }
                    }

                    out[i] = from_real<DstT>(real);
                }
                return;
            }

            if constexpr (is_fixed_point_v<SrcT>)
            {
                log_quantization_missing(src.fmt().name);
            }

            if constexpr (std::is_same_v<SrcT, DstT>)
            {
                std::copy(in, in + n, out);
            }
            else
            {
                std::transform(in, in + n, out, [](SrcT v) { return convert<DstT>(v); });
            }
        }

        static void process(encoded const& src, encoded& dst) { run(src, dst); }

        static void process(encoded::slice const& src, encoded& dst) { run(src, dst); }
    };
};

/**
 * @brief Allocates the destination of an out-of-place cast: same shape as @p from, densely laid
 *        out, dtype @p T, and no quantization (values come out dequantized).
 */
template <number T>
inline encoded::shared
make_cast_destination(std::string const& name, layout const& from)
{
    auto dense = nex::layout{from.shape()};

    std::error_code ec{};
    auto            buffer = allocate_mapped(dense, type_encoded_v<T>, ec);
    if (ec)
    {
        log_allocation_failed(name, ec.message());
        return nullptr;
    }

    return encoded::create(
        encoded::format{
            .name     = name,
            .encoding = type_encoded_v<T>,
            .layout   = std::move(dense),
            .quant    = std::nullopt,
        },
        std::move(buffer));
}

/**
 * @brief Rejects a source whose encoding has no fixed element size.
 *
 * A sub-byte code is not addressable as an element, so there is no pointer for a cast to walk:
 * ptr<T>() would hand out the placeholder type the dispatcher maps the encoding onto and read two
 * codes as one. dispatch() refuses these encodings as well, but for its own reason -- it has no
 * type to instantiate the cast with -- and answers with a message about EDataType rather than
 * about the cast. Saying it here names the fact the caller can act on, and keeps it true of every
 * entry point rather than of whichever ones happen to reach a dispatch.
 */
inline void
check_cast_source(encoded::format const& fmt)
{
    if (not has_fixed_element_size(fmt.encoding))
    {
        throw std::invalid_argument("cannot cast '" + fmt.name + "': " + to_string(fmt.encoding) +
                                    " has no fixed element size");
    }
}

/**
 * @brief Shared precondition check for the in-place casts.
 */
template <number T>
inline void
check_cast_destination(encoded const& dst, std::size_t n_element)
{
    if (dst.dtype() != type_encoded_v<T>)
    {
        throw std::invalid_argument("cast destination encoding does not match the requested type");
    }
    if (dst.n_element() != n_element)
    {
        throw std::invalid_argument("cast destination element count does not match the source");
    }
    // The destination is an encoded, which cannot be built non-contiguous, so there is nothing
    // left to check about its layout.
}

} // namespace detail

template <number T>
encoded::shared
encoded::cast() const
{
    static_assert(not is_fixed_point_v<T>,
                  "cast<T>() cannot invent quantization parameters for a fixed-point destination; "
                  "allocate an encoded whose format carries the target quantization and use "
                  "cast<T>(encoded::shared) instead");

    detail::check_cast_source(m_format);

    auto out = detail::make_cast_destination<T>(m_format.name, m_format.layout);
    if (not out)
    {
        return nullptr;
    }

    dispatch<detail::cast_to<T>::template from>(m_format.encoding, *this, *out);
    return out;
}

template <number T>
void
encoded::cast(encoded::shared buffer) const
{
    detail::check_cast_source(m_format);

    if (not buffer)
    {
        throw std::invalid_argument("cast destination must not be null");
    }
    detail::check_cast_destination<T>(*buffer, n_element());

    dispatch<detail::cast_to<T>::template from>(m_format.encoding, *this, *buffer);
}

template <number T>
encoded::shared
encoded::slice::cast() const
{
    static_assert(not is_fixed_point_v<T>,
                  "cast<T>() cannot invent quantization parameters for a fixed-point destination; "
                  "allocate an encoded whose format carries the target quantization and use "
                  "cast<T>(encoded::shared) instead");

    detail::check_cast_source(m_format);

    if (not m_format.layout.is_contiguous())
    {
        return contiguous()->cast<T>();
    }

    auto out = detail::make_cast_destination<T>(m_format.name, m_format.layout);
    if (not out)
    {
        return nullptr;
    }

    dispatch<detail::cast_to<T>::template from>(m_format.encoding, *this, *out);
    return out;
}

template <number T>
void
encoded::slice::cast(encoded::shared buffer) const
{
    detail::check_cast_source(m_format);

    if (not buffer)
    {
        throw std::invalid_argument("cast destination must not be null");
    }
    detail::check_cast_destination<T>(*buffer, n_element());

    if (not m_format.layout.is_contiguous())
    {
        contiguous()->cast<T>(std::move(buffer));
        return;
    }

    dispatch<detail::cast_to<T>::template from>(m_format.encoding, *this, *buffer);
}

} // namespace nex
