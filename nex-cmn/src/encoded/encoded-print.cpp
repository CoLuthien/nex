#include "cmn/encoded/encoded-print.hpp"

#include "cmn/dispatch.hpp"
#include "cmn/floats/f16.hpp"
#include "cmn/floats/bf16.hpp"

#include <cmath>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace nex
{
namespace detail
{
namespace
{

// Entries kept at each edge of a dimension that is elided.
constexpr std::size_t kEdgeItems = 4;

template <typename T>
void
write_real(std::ostream& os, T value)
{
    if (std::isnan(value))
    {
        os << "nan";
    }
    else if (std::isinf(value))
    {
        os << (value > 0 ? "inf" : "-inf");
    }
    else
    {
        os << value;
    }
}

/**
 * @brief Writes the one element stored at @p at. Dispatched on the encoding, so every type the
 *        module knows how to read is covered here and nowhere else.
 */
template <typename T>
struct write_element
{
    static void process(std::ostream& os, std::byte const* at)
    {
        auto const value = *reinterpret_cast<T const*>(at);

        if constexpr (is_half_precision_v<T>)
        {
            write_real(os, as_f32(value));
        }
        else if constexpr (is_fixed_point_v<T>)
        {
            os << static_cast<unsigned>(value.value);
        }
        else if constexpr (std::is_floating_point_v<T>)
        {
            write_real(os, value);
        }
        else if constexpr (one_of<T, std::int8_t, std::uint8_t>)
        {
            // A byte-wide integer would otherwise reach the stream as a character.
            os << static_cast<int>(value);
        }
        else
        {
            os << value;
        }
    }
};

struct region
{
    std::byte const* base; // element zero
    nex::layout      layout;
    EDataType        encoding;
    std::size_t      elem_size;
    bool             elide; // the region is larger than the caller's threshold
};

/**
 * @brief Byte distance from element zero to @p indices.
 *
 * The layout's own offset is deliberately not applied: both encoded and encoded::slice hand out a
 * pointer that already stands at their element zero, displacement being carried by the buffer --
 * the same contract strided-copy.hpp works under.
 */
std::size_t
byte_offset(region const& of, std::vector<std::size_t> const& indices)
{
    std::size_t elements = 0;
    for (layout::rank_type d = 0; d < indices.size(); ++d)
    {
        elements += indices[d] * static_cast<std::size_t>(of.layout.stride(d));
    }
    return elements * of.elem_size;
}

void
write_dim(std::ostream&             os,
          region const&             of,
          std::vector<std::size_t>& indices,
          layout::rank_type         dim,
          std::string const&        indent)
{
    auto const& shape = of.layout.shape();

    if (dim == shape.size())
    {
        dispatch<write_element>(of.encoding, os, of.base + byte_offset(of, indices));
        return;
    }

    auto const extent = static_cast<std::size_t>(shape[dim]);
    auto const nested = dim + 1 < shape.size(); // an inner dimension follows, so break lines
    auto const elide  = of.elide and extent > 2 * kEdgeItems;

    auto const separate = [&] {
        if (nested)
            os << ",\n" << indent << ' ';
        else
            os << ", ";
    };

    auto const write_at = [&](std::size_t i) {
        indices[dim] = i;
        write_dim(os, of, indices, dim + 1, indent + ' ');
    };

    os << '[';

    if (elide)
    {
        for (std::size_t i = 0; i < kEdgeItems; ++i)
        {
            if (i > 0) separate();
            write_at(i);
        }

        os << ", ...";
        separate();

        for (auto i = extent - kEdgeItems; i < extent; ++i)
        {
            if (i > extent - kEdgeItems) separate();
            write_at(i);
        }
    }
    else
    {
        for (std::size_t i = 0; i < extent; ++i)
        {
            if (i > 0) separate();
            write_at(i);
        }
    }

    os << ']';
}

std::string
shape_to_string(layout::shape_container const& shape)
{
    std::ostringstream oss;
    oss << '(';
    for (std::size_t i = 0; i < shape.size(); ++i)
    {
        if (i > 0) oss << ", ";
        oss << shape[i];
    }
    oss << ')';
    return oss.str();
}

/**
 * @brief The first element of a scale / zero_point operand, or "?" when it cannot be read.
 */
std::string
first_element(encoded::shared const& of)
{
    if (not of or of->ptr() == nullptr or of->n_element() == 0 or
        not has_fixed_element_size(of->dtype()))
    {
        return "?";
    }

    std::ostringstream oss;
    dispatch<write_element>(of->dtype(), oss, of->ptr());
    return oss.str();
}

/**
 * @brief Summary of a quantization: the parameters themselves for per-tensor, where to find them
 *        for per-channel. Printing a whole per-channel scale would bury the values it describes.
 */
std::string
quantization_to_string(encoded::quantization const& quant)
{
    std::ostringstream oss;

    if (quant.axis)
    {
        oss << "per-channel(axis=" << *quant.axis
            << ", groups=" << (quant.scale ? quant.scale->n_element() : 0) << ')';
    }
    else
    {
        oss << "per-tensor(scale=" << first_element(quant.scale)
            << ", zero_point=" << (quant.zero_point ? first_element(quant.zero_point) : "0") << ')';
    }

    return oss.str();
}

void
write_encoded(std::ostream&          os,
              std::string_view       label,
              encoded::format const& format,
              std::byte const*       base,
              std::size_t            threshold)
{
    auto const& shape = format.layout.shape();

    std::ostringstream head;
    head << label << '(';
    if (not format.name.empty()) head << '\'' << format.name << "', ";

    auto const prefix = head.str();
    os << prefix;

    if (base == nullptr)
    {
        os << "<null>";
    }
    else if (not has_fixed_element_size(format.encoding))
    {
        os << '<' << to_string(format.encoding) << " has no fixed element size>";
    }
    else if (format.layout.element_count() == 0)
    {
        os << "[]";
    }
    else
    {
        region of{
            .base      = base,
            .layout    = format.layout,
            .encoding  = format.encoding,
            .elem_size = in_bytes(format.encoding),
            .elide     = format.layout.element_count() > threshold,
        };

        std::vector<std::size_t> indices(shape.size(), 0);
        write_dim(os, of, indices, 0, std::string(prefix.size(), ' '));
    }

    os << ", shape=" << shape_to_string(shape) << ", dtype=" << to_string(format.encoding);
    if (format.quant) os << ", quant=" << quantization_to_string(*format.quant);
    os << ')';
}

} // namespace
} // namespace detail

std::ostream&
operator<<(std::ostream& os, encoded const& value)
{
    detail::write_encoded(os, "encoded", value.fmt(), value.ptr(), detail::kPrintThreshold);
    return os;
}

std::ostream&
operator<<(std::ostream& os, encoded::slice const& value)
{
    detail::write_encoded(os, "encoded::slice", value.fmt(), value.ptr(), detail::kPrintThreshold);
    return os;
}

std::string
to_string(encoded const& value, std::size_t threshold)
{
    std::ostringstream oss;
    detail::write_encoded(oss, "encoded", value.fmt(), value.ptr(), threshold);
    return oss.str();
}

std::string
to_string(encoded::slice const& value, std::size_t threshold)
{
    std::ostringstream oss;
    detail::write_encoded(oss, "encoded::slice", value.fmt(), value.ptr(), threshold);
    return oss.str();
}

} // namespace nex
