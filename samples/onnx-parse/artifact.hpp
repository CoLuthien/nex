
#pragma once

#include "nex/frontend/network-description.hpp"

#include <filesystem>
#include <string>
#include <vector>

namespace sample
{
namespace fs = std::filesystem;

/**
 * @brief What the sample was asked to read, and how loudly.
 */
struct options
{
    lnpu::nex::nx::onnx_source source{};

    /// How many layers of the graph to print before the checks run; 0 prints none.
    std::size_t listed = 12;

    /// How many elements of one weight the value checks look at, or 0 for every one of them.
    std::size_t sampled = 4096;

    /// Let the loader's own spdlog warnings through.
    bool verbose = false;
};

/**
 * @brief Fills in @p how from the command line, resolving what was left out by convention.
 *
 * Only the model is required. Its weight files and its encodings sit beside it under names an
 * export writes by habit -- "<model>.data" or any sibling "*.onnx.data", and "<stem>.encodings" --
 * so they are looked for there rather than asked for, and either can still be given explicitly.
 *
 * @return false when the arguments do not name a readable model, after printing why.
 */
bool arrange(int argc, char** argv, options& how);

/// One line per file the load will actually touch.
void announce(options const& how);

} // namespace sample
