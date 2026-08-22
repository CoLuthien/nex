
#pragma once

#include "nex-api.hpp"

#include "nex/frontend/layer-description.hpp"
#include "nex/nex-types.hpp"

#include <filesystem>
#include <memory>
#include <span>
#include <vector>

namespace lnpu::nex
{

class NEX_EXPORT network_description
{
public:
    using shared = std::shared_ptr<network_description>;
    using unique = std::unique_ptr<network_description>;

    network_description()          = default;
    virtual ~network_description() = default;

    // use in place or with pointer type
    network_description(network_description const&)            = delete;
    network_description(network_description&&)                 = delete;
    network_description& operator=(network_description const&) = delete;
    network_description& operator=(network_description&&)      = delete;

    virtual std::size_t n_inputs() const  = 0;
    virtual std::size_t n_outputs() const = 0;

    virtual value_description const* input_description(std::size_t index) const  = 0;
    virtual value_description const* output_description(std::size_t index) const = 0;

    virtual value_description const* description(std::size_t index) const     = 0;
    virtual value_description const* description(std::string_view name) const = 0;

    virtual encoded::shared                            parameter(std::string_view name) const = 0;
    virtual std::span<layer_description::shared const> layers() const                         = 0;
};

namespace nx
{
namespace fs = std::filesystem;

/**
 * @brief Where one onnx graph is read from.
 *
 * A model too large for a single protobuf message keeps its weights in separate files, which
 * TensorProto::external_data then names. Those files are not resolved relative to the model on
 * their own -- list them in #external_data and the loader matches them by file name.
 */
struct onnx_source
{
    fs::path              model;
    std::vector<fs::path> external_data;

    /**
     * @brief An AIMET encodings.json describing how the values of this graph are quantized.
     *
     * A model quantized outside onnx stays a float graph and keeps its scales here, so this is
     * what puts the quantization back onto the values and the weights it belongs to. Empty for a
     * model that carries none.
     */
    fs::path encodings;
};

/**
 * @brief Reads @p source into a network description whose layers are topologically ordered.
 *
 * @throws std::runtime_error when the file cannot be read or is not a parsable onnx model.
 * @throws std::logic_error when the graph itself is inconsistent (a cycle, a value no layer and
 *         no initializer produces) or holds something this frontend cannot represent.
 */
NEX_EXPORT network_description::shared load(onnx_source const& source);

} // namespace nx
} // namespace lnpu::nex
