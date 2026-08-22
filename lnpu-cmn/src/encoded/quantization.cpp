#include "quantization.hpp"

#include "cmn/encoded/cast.hpp"
#include "cmn/dispatch.hpp"

#include <stdexcept>

namespace lnpu::detail
{
namespace
{

/// Widest code range any encoding here could hold, so a nonsense bitwidth is caught even on the
/// float region of a fake-quant, where no encoding is there to bound it.
constexpr std::uint8_t kWidestCodeRange = 32;

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
validate(encoded::quantization const& quant, EDataType encoding, std::string const& name)
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

    if (quant.bitwidth == 0 or quant.bitwidth > kWidestCodeRange)
    {
        throw std::invalid_argument("'" + name + "' has a bitwidth of " +
                                    std::to_string(quant.bitwidth) + "; a code range is 1 to " +
                                    std::to_string(kWidestCodeRange) + " bits wide");
    }

    // Only once the region holds codes does the encoding have to have room for them. On a float
    // region the parameters are fake-quant metadata describing a narrower range than the storage,
    // which is the normal state of a weight that has not been converted yet.
    if (auto const room = 8 * in_bytes(encoding); stores_codes(encoding) and quant.bitwidth > room)
    {
        throw std::invalid_argument("'" + name + "' describes a " + std::to_string(quant.bitwidth) +
                                    "-bit code range but is stored as " + to_string(encoding) +
                                    ", which holds " + std::to_string(room) + " bits");
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
