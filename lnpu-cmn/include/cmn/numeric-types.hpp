
#pragma once

#include "cmn-api.hpp"
#include "cmn/layout.hpp"
#include <cstdint>

#include <concepts>
#include <string>
#include <type_traits>
#include <vector>

namespace lnpu
{

using coordinate = std::vector<layout::extent_type>;

/**
 * @brief Whether @p T is one of @p Us. Reads at a call site as the question it asks --
 *        one_of<T, f16, bf16> -- which a fold of std::is_same_v does not.
 */
template <typename T, typename... Us>
inline constexpr bool one_of = (std::is_same_v<T, Us> or ...);

struct f16
{
    std::uint16_t value{0x0000};
};

struct bf16
{
    std::uint16_t value{0x0000};
};

struct uf8
{
    std::uint8_t value{0x00};
};

struct uf16
{
    std::uint16_t value{0x0000};
};

struct uf32
{
    std::uint32_t value{0x00000000};
};

enum class EDataType : std::uint16_t
{
    i8,
    u8,
    i16,
    u16,
    i32,
    f16,
    bf16,
    f32,

    // fixed point integer types
    // uf4,
    uf8,
    uf16, // unsigned fixed point 16
    uf32,

    // invalid
    invalid,
};

/**
 * @brief Size of one stored element, or (std::size_t)-1 for an encoding that has none.
 *
 * An encoding without a fixed element size is not addressable as an array -- a sub-byte code
 * would read two elements as one -- so every walk over stored bytes has to refuse it rather
 * than compute with the answer. has_fixed_element_size() is that question asked directly.
 */
[[nodiscard]] constexpr std::size_t
in_bytes(EDataType type)
{
    switch (type)
    {
    case EDataType::i8:
    case EDataType::u8:
    case EDataType::uf8:
        return sizeof(std::uint8_t);

    case EDataType::i16:
    case EDataType::u16:
    case EDataType::f16:
    case EDataType::bf16:
    case EDataType::uf16:
        return sizeof(std::uint16_t);

    case EDataType::i32:
    case EDataType::f32:
    case EDataType::uf32:
        return sizeof(std::uint32_t);

    case EDataType::invalid:
        break;
    }

    return static_cast<std::size_t>(-1);
}

[[nodiscard]] constexpr bool
has_fixed_element_size(EDataType type)
{
    auto const size = in_bytes(type);
    return size != 0 and size != static_cast<std::size_t>(-1);
}

[[nodiscard]] inline std::string
to_string(EDataType type)
{
    switch (type)
    {
    case EDataType::i8:
        return "i8";
    case EDataType::u8:
        return "u8";
    case EDataType::i16:
        return "i16";
    case EDataType::u16:
        return "u16";
    case EDataType::i32:
        return "i32";
    case EDataType::f16:
        return "f16";
    case EDataType::bf16:
        return "bf16";
    case EDataType::f32:
        return "f32";
    case EDataType::uf8:
        return "uf8";
    case EDataType::uf16:
        return "uf16";
    case EDataType::uf32:
        return "uf32";
    case EDataType::invalid:
        return "invalid";
    }

    return "unknown";
}

template <EDataType E>
struct encoded_type;

template <typename T>
struct type_encoded;

// clang-format off
// template <> struct encoded_type<EDataType::uf4>  { using type = std::uint8_t; };

template <> struct encoded_type<EDataType::i8>   { using type = std::int8_t; };
template <> struct encoded_type<EDataType::u8>   { using type = std::uint8_t; };
template <> struct encoded_type<EDataType::i16>  { using type = std::int16_t; };
template <> struct encoded_type<EDataType::u16>  { using type = std::uint16_t; };
template <> struct encoded_type<EDataType::i32>  { using type = std::int32_t; };
template <> struct encoded_type<EDataType::f16>  { using type = f16; };
template <> struct encoded_type<EDataType::bf16> { using type = bf16; };
template <> struct encoded_type<EDataType::f32>  { using type = float; };
template <> struct encoded_type<EDataType::uf8>  { using type = uf8; };
template <> struct encoded_type<EDataType::uf16> { using type = uf16; };
template <> struct encoded_type<EDataType::uf32> { using type = uf32; };

// clang-format on

// clang-format off
// template <> struct type_encoded<> {static constexpr auto value = EDataType::uf4};

template <> struct type_encoded<std::int8_t>   { static constexpr auto value = EDataType::i8;   };
template <> struct type_encoded<std::uint8_t>  { static constexpr auto value = EDataType::u8;   };
template <> struct type_encoded<std::int16_t>  { static constexpr auto value = EDataType::i16;  };
template <> struct type_encoded<std::uint16_t> { static constexpr auto value = EDataType::u16;  };
template <> struct type_encoded<std::int32_t>  { static constexpr auto value = EDataType::i32;  };
template <> struct type_encoded<f16>           { static constexpr auto value = EDataType::f16;  };
template <> struct type_encoded<bf16>          { static constexpr auto value = EDataType::bf16; };
template <> struct type_encoded<float>         { static constexpr auto value = EDataType::f32;  };
template <> struct type_encoded<uf8>           { static constexpr auto value = EDataType::uf8;  };
template <> struct type_encoded<uf16>          { static constexpr auto value = EDataType::uf16; };
template <> struct type_encoded<uf32>          { static constexpr auto value = EDataType::uf32; };
// clang-format on

template <EDataType E>
using encoded_type_t = typename encoded_type<E>::type;

template <typename T>
static constexpr auto type_encoded_v = type_encoded<T>::value;

template <typename T>
static constexpr bool is_fixed_point_v = std::is_same_v<T, uf8>     //
                                         or std::is_same_v<T, uf16> //
                                         or std::is_same_v<T, uf32>;
template <typename T>
static constexpr bool is_half_precision_v = std::is_same_v<f16, T> //
                                            or std::is_same_v<bf16, T>;

/**
 * @brief An element type an encoded region can store: the builtin arithmetic types plus the
 *        half-precision and unsigned fixed-point types this module carries as byte-wise structs.
 */
template <typename T>
concept number =
    std::integral<T> or std::floating_point<T> or one_of<T, f16, bf16, uf8, uf16, uf32>;

static_assert(sizeof(f16) == sizeof(std::uint16_t));
static_assert(sizeof(bf16) == sizeof(std::uint16_t));

} // namespace lnpu
