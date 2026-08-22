
#pragma once

#include "nex/nex-types.hpp"

#include <filesystem>
#include <string>
#include <unordered_map>

/*
 * Nothing here is specific to onnx -- an encodings file names tensors, and every frontend has
 * those. It lives under the onnx frontend only because that is the one loader reading it today;
 * move it up to frontend/ once a second one needs it.
 */
namespace lnpu::nex::nx
{
namespace fs = std::filesystem;

/// Quantization parameters by the name of the value they belong to.
using quantization_map = std::unordered_map<std::string, quantization>;

/**
 * @brief Reads the sidecar file an AIMET export writes next to a quantized model.
 *
 * A model quantized outside onnx keeps its graph in float and its scales in a separate json, so
 * this is what pairs the two back up: every entry names a value of the graph and carries the
 * affine mapping from stored codes to real numbers. Both sections are read, parameters first, and
 * an entry that is not an integer quantization (a float passthrough) is skipped rather than
 * turned into an identity mapping.
 *
 * Two conventions differ from ours and are converted here rather than left to every reader:
 * AIMET's `offset` is added where our zero_point is subtracted, and a symmetric quantization
 * leaves the zero point out entirely instead of storing zeros.
 *
 * @warning That second conversion places a requirement on whoever writes the codes, because it
 * makes the storage convention depend on `is_sym` -- the same file asks for two different things:
 *
 *   - asymmetric: zero_point is -offset, which reproduces AIMET's mapping over an UNSIGNED code,
 *     exactly as AIMET stores it.
 *   - symmetric: the zero point is dropped, which is that mapping only over a SIGNED code -- one
 *     the converter has shifted down by 2^(bitwidth-1) from AIMET's unsigned one.
 *
 * A converter that writes unsigned codes for a symmetric entry produces values uniformly
 * 2^(bitwidth-1) * scale too large. Nothing fails and the tensor still looks reasonable, so the
 * error survives to the accuracy numbers. Signed also happens to be what the AIE integer MACs
 * want, so it costs nothing to hold to.
 *
 * The reader defends the half it can see: an entry whose offset is not the one the shift assumes
 * is refused (see expect_shifted_offsets), which is what catches an export whose symmetry
 * settings were changed underneath us.
 *
 * @param path an encodings file in the AIMET 1.x layout. A path that does not exist yields an
 *        empty map -- having no encodings is not an error, only a model that is not quantized.
 *
 * @throws std::runtime_error when the file cannot be read, is not json, is not the 1.x layout,
 *         holds an entry whose scale and offset do not describe one affine mapping, or holds a
 *         symmetric entry in a range this reader does not convert.
 */
quantization_map read_aimet_encodings(fs::path const& path);

} // namespace lnpu::nex::nx
