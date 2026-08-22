#include "quantization.hpp"

#include "cmn/encoded/cast.hpp"
#include "cmn/dispatch.hpp"

#include <stdexcept>

namespace lnpu::detail
{
namespace
{

/**
 * @brief Reads is_quantizable<T> back out for a runtime encoding.
 */
template <typename T>
struct quantizable_probe
{
    static void process(bool& answer) { answer = is_quantizable<T>; }
};

std::string
describe(encoded::shared const& parameter)
{
    return parameter ? to_string(parameter->dtype()) : std::string{"(null)"};
}

} // namespace

bool
stores_codes(EDataType encoding)
{
    // No fixed element size, so there is nothing to read whatever the parameters say -- and no
    // type to probe the trait with either.
    if (not has_fixed_element_size(encoding))
    {
        return false;
    }

    bool quantizable = false;
    dispatch<quantizable_probe>(encoding, quantizable);
    return quantizable;
}

bool
is_quantized(encoded::format const& fmt)
{
    return fmt.quant.has_value() and stores_codes(fmt.encoding);
}

std::span<float const>
scale_of(encoded::format const& fmt)
{
    if (not is_quantized(fmt)) return {};

    auto const& scale = fmt.quant->scale;
    return {scale->ptr<float>(), scale->n_element()};
}

std::span<std::int32_t const>
zero_point_of(encoded::format const& fmt)
{
    if (not is_quantized(fmt)) return {};

    auto const& zero_point = fmt.quant->zero_point;
    if (not zero_point) return {}; // symmetric: every entry would be zero

    return {zero_point->ptr<std::int32_t>(), zero_point->n_element()};
}

void
validate(encoded::quantization const& quant, std::string const& name)
{
    if (not quant.scale)
    {
        throw std::invalid_argument("'" + name + "' carries a quantization without a scale");
    }

    if (quant.scale->dtype() != EDataType::f32)
    {
        throw std::invalid_argument("'" + name + "' has a " + describe(quant.scale) +
                                    " scale; a quantization scale is normalized to f32");
    }

    if (not quant.zero_point) return;

    if (quant.zero_point->dtype() != EDataType::i32)
    {
        throw std::invalid_argument("'" + name + "' has a " + describe(quant.zero_point) +
                                    " zero_point; a zero point is normalized to i32");
    }

    if (quant.zero_point->shape() != quant.scale->shape())
    {
        throw std::invalid_argument("'" + name +
                                    "' has a zero_point whose shape does not match its scale");
    }
}

} // namespace lnpu::detail
