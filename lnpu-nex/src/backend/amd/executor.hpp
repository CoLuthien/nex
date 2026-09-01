

#pragma once

#include "layer.hpp"
#include "device.hpp"

#include "nex/backend/network.hpp"
#include "nex/frontend/network-description.hpp"
#include <system_error>

namespace lnpu::nex::amd
{

class executor : public nex::executor
{
    amd::device*               m_dev;
    std::vector<layer::unique> m_layers;

public:
    explicit executor(network_description const& description, amd::device* dev);

    std::error_code execute() override;
};

} // namespace lnpu::nex::amd
