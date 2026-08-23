#pragma once

// One entry point per operator. Each owns the whole run for its designs: reading what it needs,
// wiring, comparing against aiecc, opening the array, and checking the numbers. They share the
// support header and nothing else, so adding an operator is adding a file.

#include "support.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace hw
{

/// One rmsnorm design, which is one case: its length is baked in.
bool run_rmsnorm(fs::path const& xclbin, fs::path const& reference, options const& how);

/// A shape a shape-agnostic design was given, and the stream aiecc produced for it.
struct gemm_case
{
    std::uint32_t m{};
    std::uint32_t k{};
    std::uint32_t n{};
    fs::path      reference;
};

/**
 * @brief Every shape one gemm design was asked for, against that one design.
 *
 * Taken together rather than one at a time on purpose: the array is opened once and every shape
 * runs on it, which is the claim the design makes -- one xclbin, every projection -- being tested
 * rather than assumed.
 *
 * @return how many of @p shapes passed.
 */
std::size_t run_gemm(fs::path const& xclbin, std::vector<gemm_case> const& shapes, options const& how);

} // namespace hw
