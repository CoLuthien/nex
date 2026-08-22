
#pragma once

#include "quantization-encoding.hpp"

#include "nex/nex-types.hpp"

#include "cmn/buffer/buffer-interface.hpp"
#include "cmn/encoded/encoded.hpp"
#include "cmn/numeric-types.hpp"

#include <onnx.pb.h>

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * Everything that turns onnx protobuf messages into the types this runtime speaks. Kept apart
 * from the graph assembly in network-description.cpp so that onnx.pb.h stays out of it.
 */
namespace lnpu::nex::nx::decode
{

/// Weight files named the way TensorProto::external_data spells them (file name, not path).
using external_files = std::unordered_map<std::string, buffer::shared>;

/// Initializers by the name the graph refers to them under.
using tensor_map = std::unordered_map<std::string, encoded::shared>;

template <typename T>
using repeated = ::google::protobuf::RepeatedPtrField<T>;

/**
 * @brief The encoding an onnx element type is stored as here.
 *
 * The answer is not always the same type: this runtime has no 64-bit and no double encoding, so
 * int64/uint64/uint32 arrive as i32, double as f32, and bool as u8 -- see tensor() for the range
 * checking that comes with it. An onnx type with no counterpart at all (string, complex, the
 * float8/float4 family, int2/uint2) has no answer.
 *
 * @throws std::logic_error for an element type this runtime cannot store.
 */
EDataType as_encoding(std::int32_t onnx_type);

/**
 * @brief Whether importing @p onnx_type rewrites the payload element by element.
 *
 * False means the stored bytes are already what we would produce, which is what lets an external
 * weight be handed out as a window onto the mapped file instead of a copy.
 */
bool is_transcoded(std::int32_t onnx_type);

/**
 * @brief Materialises one TensorProto -- an initializer or the value of a TENSOR attribute.
 *
 * The payload is taken from whichever of the three places onnx allows: raw_data, one of the typed
 * repeated fields, or a window of an external file listed in @p externals. @p quant travels with
 * the region for the whole of its life, so it is given here rather than set afterwards.
 *
 * @throws std::runtime_error when the payload is missing, sized differently from the shape, or a
 *         value does not survive the narrowing described in as_encoding().
 */
encoded::shared tensor(::onnx::TensorProto const&  proto,
                       external_files const&       externals,
                       std::optional<quantization> quant = std::nullopt);

/**
 * @brief Every initializer of a graph, keyed by name.
 *
 * A weight named in @p encodings is built carrying its quantization. On a float weight that is
 * fake-quant metadata -- which is exactly what an AIMET export describes -- and encoded says so
 * itself through is_quantized().
 */
tensor_map initializers(repeated<::onnx::TensorProto> const& protos,
                        external_files const&                externals,
                        quantization_map const&              encodings);

/**
 * @brief Value descriptions for graph inputs, outputs or value_info entries.
 *
 * Neither the element type nor the shape is required to be there: onnx leaves both out for a
 * value it has not inferred, and a symbolic dimension ("batch") becomes a negative extent, which
 * is how lnpu::layout spells "not resolved yet".
 */
std::vector<value_description> values(repeated<::onnx::ValueInfoProto> const& protos,
                                      quantization_map const&                 encodings);

/// The attributes of one node, in the order the node lists them.
std::vector<attribute> attributes(repeated<::onnx::AttributeProto> const& protos);

} // namespace lnpu::nex::nx::decode
