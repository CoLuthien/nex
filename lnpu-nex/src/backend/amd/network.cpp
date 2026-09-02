
#include "network.hpp"

namespace lnpu::nex::amd
{

network::network(std::string_view             name,
                 network_description::shared  description,
                 std::shared_ptr<amd::device> dev)
    : m_description(description), //
      m_device(dev),              //
      m_name(name)                //
{
}
} // namespace lnpu::nex::amd
