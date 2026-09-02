
#pragma once

#include "operation.hpp"

#include "device.hpp"

#include "nex/backend/network.hpp"
#include "nex/frontend/network-description.hpp"
#include <memory>
#include <system_error>

namespace lnpu::nex::amd
{

class network : public nex::network
{
    network_description::shared  m_description;
    std::shared_ptr<amd::device> m_device;
    std::string                  m_name;

    std::vector<encoded::format> m_inputs;
    std::vector<encoded::format> m_outputs;

public:
    explicit network(std::string_view             name,
                     network_description::shared  description,
                     std::shared_ptr<amd::device> device);

    executor::unique prepare(std::error_code& ec) override;
};

} // namespace lnpu::nex::amd
