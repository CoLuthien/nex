
#pragma once

#include "cmn/layout.hpp"
#include "cmn/numeric-types.hpp"

#include "cmn/buffer/buffer-interface.hpp"

#include "nex/backend/network.hpp"
#include "nex/frontend/network-description.hpp"

#include <memory>

namespace lnpu::nex
{

class device
{
public:
    using shared = std::shared_ptr<device>;

    virtual network::shared create_network(std::string_view            name,
                                           network_description::shared description) = 0;
    virtual buffer::shared  allocate(EDataType type, lnpu::layout layout)           = 0;
};
} // namespace lnpu::nex
