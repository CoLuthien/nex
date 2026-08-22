
#include "layer-description.hpp"

#include <stdexcept>
#include <utility>

namespace lnpu::nex::nx
{
namespace
{

/// The name of a value, or nothing at all for an optional operand the node left out.
std::string_view
name_of(value_description const* value)
{
    return value != nullptr ? std::string_view{value->name} : std::string_view{};
}

template <typename T>
T const&
at(std::vector<T> const& entries, std::size_t index, char const* what)
{
    if (index >= entries.size())
    {
        throw std::out_of_range("[nx::nx_layer] no " + std::string{what} + " at index " +
                                std::to_string(index) + " (there are " +
                                std::to_string(entries.size()) + ")");
    }

    return entries[index];
}

} // namespace

nx_layer::nx_layer(descriptor&& described)
    : m_name(std::move(described.name)),             //
      m_op_type(std::move(described.op_type)),       //
      m_domain(std::move(described.domain)),         //
      m_opset(described.opset),                      //
      m_schema(described.schema),                    //
      m_operands(std::move(described.operands)),     //
      m_results(std::move(described.results)),       //
      m_attributes(std::move(described.attributes)), //
      m_parameters(std::move(described.parameters))
{
}

layer_schema const*
nx_layer::schema() const
{
    return m_schema;
}

std::string_view
nx_layer::name() const
{
    return m_name;
}

std::size_t
nx_layer::input_count() const
{
    return m_operands.size();
}

std::size_t
nx_layer::output_count() const
{
    return m_results.size();
}

std::string_view
nx_layer::input_name(std::size_t index) const
{
    return name_of(at(m_operands, index, "operand"));
}

std::string_view
nx_layer::output_name(std::size_t index) const
{
    return name_of(at(m_results, index, "result"));
}

value_description const*
nx_layer::input(std::size_t index) const
{
    return at(m_operands, index, "operand");
}

value_description const*
nx_layer::output(std::size_t index) const
{
    return at(m_results, index, "result");
}

value_description const*
nx_layer::input(std::string_view name) const
{
    for (auto const* operand : m_operands)
    {
        if (name_of(operand) == name)
        {
            return operand;
        }
    }

    return nullptr;
}

value_description const*
nx_layer::output(std::string_view name) const
{
    for (auto const* result : m_results)
    {
        if (name_of(result) == name)
        {
            return result;
        }
    }

    return nullptr;
}

std::size_t
nx_layer::parameter_count() const
{
    return m_parameters.size();
}

std::string_view
nx_layer::parameter_name(std::size_t index) const
{
    return at(m_parameters, index, "parameter")->fmt().name;
}

encoded::shared
nx_layer::parameter(std::size_t index) const
{
    return at(m_parameters, index, "parameter");
}

encoded::shared
nx_layer::parameter(std::string_view name) const
{
    for (auto const& weight : m_parameters)
    {
        if (weight->fmt().name == name)
        {
            return weight;
        }
    }

    return nullptr;
}

std::span<attribute const>
nx_layer::attributes() const
{
    return m_attributes;
}

attribute const*
nx_layer::find_attribute(std::string_view name) const
{
    for (auto const& attr : m_attributes)
    {
        if (attr.name == name)
        {
            return &attr;
        }
    }

    return nullptr;
}

} // namespace lnpu::nex::nx
