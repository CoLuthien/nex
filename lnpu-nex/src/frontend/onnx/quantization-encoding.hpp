
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
 * @param path an encodings.json in the AIMET 1.x layout. A path that does not exist yields an
 *        empty map -- having no encodings is not an error, only a model that is not quantized.
 *
 * @throws std::runtime_error when the file cannot be read, is not json, is not the 1.x layout, or
 *         holds an entry whose scale and offset do not describe one affine mapping.
 */
quantization_map read_aimet_encodings(fs::path const& path);

} // namespace lnpu::nex::nx
