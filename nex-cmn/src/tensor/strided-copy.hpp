
#pragma once

#include "cmn/layout.hpp"
#include "cmn/numeric-types.hpp"

namespace nex::detail
{

void strided_copy(std::byte const* src_base,
                  layout const&    src_layout,
                  std::byte*       dst_base,
                  layout const&    dst_layout,
                  EDataType        dtype);
}
