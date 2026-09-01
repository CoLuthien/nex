
#pragma once

#include "nex/frontend/layer-schema.hpp"
#include "nex/frontend/layer-description.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace lnpu::nex::nx
{

/**
 * @brief One onnx node, resolved against the graph it came from.
 *
 * Nothing here is owned twice: the operands and results point into the value table of the network
 * that built the layer, and the parameters are the very tensors the network keeps its
 * initializers in. A layer therefore only stays valid as long as that network does.
 */
class nx_layer final : public layer_description
{
public:
    struct descriptor
    {
        std::string  name;
        std::string  op_type;
        std::string  domain;
        std::int64_t opset = 0;

        /// The signature this node was resolved to, or null when the operator is unknown at
        /// #opset -- see schema_registry::find().
        layer_schema const* schema = nullptr;

        /// One entry per operand the node lists, null where it left an optional one out.
        std::vector<value_description> operands;
        std::vector<value_description> results;

        std::vector<attribute>       attributes;
        std::vector<encoded::shared> parameters;
    };

    explicit nx_layer(descriptor&& described);
    ~nx_layer() override = default;

    layer_schema const* schema() const override;
    std::string_view    name() const override;

    /// What the node calls itself in onnx terms; not part of the frontend interface.
    std::string_view op_type() const override { return m_op_type; }
    std::string_view domain() const { return m_domain; }
    std::int64_t     opset() const { return m_opset; }

    std::size_t input_count() const override;
    std::size_t output_count() const override;

    std::string_view input_name(std::size_t index) const override;
    std::string_view output_name(std::size_t index) const override;

    value_description const* input(std::size_t index) const override;
    value_description const* output(std::size_t index) const override;

    value_description const* input(std::string_view name) const override;
    value_description const* output(std::string_view name) const override;

    std::size_t      parameter_count() const override;
    std::string_view parameter_name(std::size_t index) const override;

    encoded::shared parameter(std::size_t index) const override;
    encoded::shared parameter(std::string_view name) const override;

    std::span<attribute const> attributes() const override;
    attribute const*           find_attribute(std::string_view name) const override;

private:
    std::string  m_name;
    std::string  m_op_type;
    std::string  m_domain;
    std::int64_t m_opset;

    layer_schema const* m_schema;

    std::vector<value_description> m_operands;
    std::vector<value_description> m_results;

    std::vector<attribute>       m_attributes;
    std::vector<encoded::shared> m_parameters;
};

} // namespace lnpu::nex::nx
