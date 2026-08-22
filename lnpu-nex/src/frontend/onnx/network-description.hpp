
#pragma once

#include "nex-api.hpp"

#include "nex/frontend/network-description.hpp"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace lnpu::nex::nx
{

/**
 * @brief One onnx graph, read once and then only looked at.
 *
 * Every value the graph mentions -- its inputs, its outputs, its initializers and every edge
 * between two nodes -- lives exactly once in #contents::values, and everything else refers to it
 * by index or by pointer. A graph input and the same tensor seen from inside a layer are
 * therefore the same object, and a shape settled on one of them is settled for all of them.
 */
class NEX_EXPORT nx_network final : public network_description
{
public:
    /// The finished graph. Assembled in network-description.cpp, where onnx.pb.h is in scope.
    struct contents
    {
        std::vector<value_description>               values;
        std::unordered_map<std::string, std::size_t> by_name;

        /// Positions in #values, in the order the graph lists them.
        std::vector<std::size_t> inputs;
        std::vector<std::size_t> outputs;

        std::unordered_map<std::string, encoded::shared> weights;

        /// Producers before consumers, whatever order the file had them in.
        std::vector<layer_description::shared> layers;
    };

    explicit nx_network(contents&& parts);
    ~nx_network() override;

    std::size_t n_inputs() const override;
    std::size_t n_outputs() const override;

    value_description const* input_description(std::size_t index) const override;
    value_description const* output_description(std::size_t index) const override;

    value_description const* description(std::size_t index) const override;
    value_description const* description(std::string_view name) const override;

    /// How many values the graph mentions in total, which is what description(index) indexes.
    std::size_t n_descriptions() const { return m_contents.values.size(); }

    encoded::shared                            parameter(std::string_view name) const override;
    std::span<layer_description::shared const> layers() const override;

private:
    contents m_contents;
};

} // namespace lnpu::nex::nx
