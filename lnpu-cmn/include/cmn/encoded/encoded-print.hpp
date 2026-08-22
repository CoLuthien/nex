#pragma once

#include "cmn-api.hpp"
#include "cmn/encoded/encoded.hpp"
#include "cmn/encoded/encoded-slice.hpp"

#include <cstddef>
#include <ostream>
#include <string>

namespace lnpu
{

namespace detail
{
/**
 * @brief Element count above which every dimension is printed as its first and last few entries
 *        with the middle elided.
 */
inline constexpr std::size_t kPrintThreshold = 128;
} // namespace detail

/**
 * @brief Human-readable dump of an encoded and of a view into one.
 *
 * The values are shown as they are STORED -- a quantized encoded prints its codes, not the reals
 * they stand for, with its scale/zero_point summarised in the trailer. Use cast<T>() first to see
 * dequantized values.
 *
 * Neither overload throws: a null buffer or an encoding without a fixed element size is reported
 * in place of the data.
 */
CMN_EXPORT std::ostream& operator<<(std::ostream& os, encoded const& value);
CMN_EXPORT std::ostream& operator<<(std::ostream& os, encoded::slice const& value);

[[nodiscard]] CMN_EXPORT std::string to_string(encoded const& value,
                                               std::size_t    threshold = detail::kPrintThreshold);

[[nodiscard]] CMN_EXPORT std::string to_string(encoded::slice const& value,
                                               std::size_t threshold = detail::kPrintThreshold);

} // namespace lnpu
