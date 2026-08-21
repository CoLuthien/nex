
#pragma once

#include "cmn-api.hpp"
#include "cmn/numeric-types.hpp"

#include <stdexcept>
#include <utility>

namespace nex
{

/**
 * @brief Runs F<T>::process(args...) with T the element type @p type stores.
 *
 * This is the one place a runtime encoding becomes a compile-time type, so anything that has to
 * read stored elements -- casting, printing, probing a type trait -- is written once as a
 * template and handed here.
 *
 * @throws std::invalid_argument for an encoding with no element type to instantiate with, which
 *         is exactly the set has_fixed_element_size() rejects.
 */
template <template <typename> typename F, typename... Args>
void
dispatch(EDataType type, Args&&... args)
{
    switch (type)
    {
    case EDataType::i8:
        F<encoded_type_t<EDataType::i8>>::process(std::forward<Args>(args)...);
        return;
    case EDataType::u8:
        F<encoded_type_t<EDataType::u8>>::process(std::forward<Args>(args)...);
        return;
    case EDataType::i16:
        F<encoded_type_t<EDataType::i16>>::process(std::forward<Args>(args)...);
        return;
    case EDataType::u16:
        F<encoded_type_t<EDataType::u16>>::process(std::forward<Args>(args)...);
        return;
    case EDataType::i32:
        F<encoded_type_t<EDataType::i32>>::process(std::forward<Args>(args)...);
        return;
    case EDataType::f16:
        F<encoded_type_t<EDataType::f16>>::process(std::forward<Args>(args)...);
        return;
    case EDataType::bf16:
        F<encoded_type_t<EDataType::bf16>>::process(std::forward<Args>(args)...);
        return;
    case EDataType::f32:
        F<encoded_type_t<EDataType::f32>>::process(std::forward<Args>(args)...);
        return;
    case EDataType::uf8:
        F<encoded_type_t<EDataType::uf8>>::process(std::forward<Args>(args)...);
        return;
    case EDataType::uf16:
        F<encoded_type_t<EDataType::uf16>>::process(std::forward<Args>(args)...);
        return;
    case EDataType::uf32:
        F<encoded_type_t<EDataType::uf32>>::process(std::forward<Args>(args)...);
        return;

    case EDataType::invalid:
        break;
    }

    throw std::invalid_argument("dispatch: " + to_string(type) + " has no element type to read");
}

} // namespace nex
