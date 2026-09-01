
#include "executor.hpp"
#include "operation.hpp"

#include <stdexcept>
#include <string>

namespace lnpu::nex::amd
{

executor::executor(network_description const& description, amd::device* dev) //
    : m_dev(dev)
{
    // 'node' rather than 'layer': a loop variable of that name hides the class of the same name,
    // and layer::create() below would then not name a type at all.
    for (auto& node : description.layers())
    {
        std::error_code ec{};

        auto* op = m_dev->op(node->op_type());
        if (not op)
        {
            throw std::runtime_error("[nex::amd::executor] no operation is registered for '" +
                                     std::string{node->op_type()} + "', which '" +
                                     std::string{node->name()} + "' is");
        }

        auto executable = op->create_instance(node, ec);

        if (ec)
        {
            // create_instance() has already said which of its three refusals this was.
            throw std::runtime_error("[nex::amd::executor] '" + std::string{node->name()} +
                                     "' would not lower: " + ec.message());
        }

        // create() logs why before it hands back nothing, so there is nothing to add here -- but
        // an unchecked nullptr would sit in the vector until execute() dereferenced it.
        auto ready = layer::create(*node, std::move(executable));
        if (nullptr == ready)
        {
            throw std::runtime_error("[nex::amd::executor] '" + std::string{node->name()} +
                                     "' and the program lowered for it do not agree");
        }

        m_layers.emplace_back(std::move(ready));
    }
}

std::error_code
executor::execute()
{
    for (auto& one : m_layers)
    {
        if (auto const ec = one->execute())
        {
            return ec;
        }
    }

    return {};
}
} // namespace lnpu::nex::amd
