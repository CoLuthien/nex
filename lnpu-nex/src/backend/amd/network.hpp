
#pragma once

#include "operation.hpp"

#include "device.hpp"

#include "nex/backend/network.hpp"
#include "nex/frontend/network-description.hpp"
#include <system_error>

namespace lnpu::nex::amd
{

class network : public nex::network
{
    network_description::shared m_description;

public:
    explicit network(std::string_view name, network_description::shared description);

    executor::unique prepare(std::error_code& ec) override;
};

} // namespace lnpu::nex::amd
