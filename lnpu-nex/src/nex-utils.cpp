
#include "nex/nex-types.hpp"

namespace lnpu::nex
{

/**
 * @brief What the graph states about a boundary value, in the terms io_contract answers in.
 *
 * A value_description leaves its fields optional because a graph need not carry them, and a
 * format does not. What the graph did not say becomes the value that says so -- an invalid
 * encoding, a layout of rank zero -- rather than a guess, so a caller can still tell the two
 * apart.
 */
encoded::format
stated_as(value_description const& value)
{
    return encoded::format{
        .name     = value.name,
        .encoding = value.dtype.value_or(EDataType::invalid),
        .layout   = value.layout.value_or(lnpu::layout{}),
        .quant    = value.quant,
    };
}
} // namespace lnpu::nex
