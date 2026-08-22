
#pragma once

#include "cmn/numeric-types.hpp"
#include "cmn/encoded/encoded.hpp"

#include <optional>
#include <string>
#include <variant>

namespace lnpu
{

using quantization = encoded::quantization;

struct value_description
{
    std::string                 name;
    std::optional<EDataType>    dtype  = std::nullopt;
    std::optional<lnpu::layout> layout = std::nullopt;
    std::optional<quantization> quant  = std::nullopt;
};

using attribute_value = std::variant<float,
                                     std::int64_t,
                                     std::string,
                                     std::vector<float>,
                                     std::vector<std::int64_t>,
                                     std::vector<std::string>,
                                     encoded::shared>;
struct attribute
{
    std::string     name;
    attribute_value value;
};

} // namespace lnpu
