#pragma once

// What every case needs and none of them should each write again: bf16, file reading, the
// comparison against a reference, and the page-aligned storage a kernel argument is pinned from.

#include "amd/command.hpp"

#include <xrt/xrt_bo.h>
#include <xrt/xrt_device.h>
#include <xrt/experimental/xrt_xclbin.h>

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <memory>
#include <span>
#include <string>
#include <vector>

namespace hw
{
namespace fs = std::filesystem;
using lnpu::nex::amd::command;

/// How a run was asked for.
struct options
{
    fs::path dir;
    int      repeats{100};
    bool     dry{false};

    /// Only cases whose label contains this. Empty runs everything.
    std::string only;

    /**
     * @brief Open a hardware context per case rather than per design.
     *
     * Off, a design's shapes share one context, which is the arrangement the runtime will use and
     * the claim the design makes. On, each shape gets its own, which is what separates "this
     * shape cannot run" from "this shape cannot run after that one". The device is the same
     * either way -- it is the context that is in question.
     *
     * A diagnostic, not a pattern to copy. A context per shape does not scale: the driver holds
     * sixteen at once and a model has far more shapes than that. If it turns out to be the only
     * way a shape runs, that is a cost to plan around, not an arrangement to adopt.
     */
    bool isolate{false};

    /**
     * @brief Have a gemm wait for its inputs as well as its result.
     *
     * See gemm::parameters::wait_for_inputs. The stream stops matching aiecc when this is on --
     * it is deliberately a superset -- so the reference comparison is skipped and said to be.
     */
    bool drain{false};

    /// Cases to run, in this order, repeats included. Empty runs each once in name order.
    std::vector<std::string> order;

    /// Times to run and measure each gemm shape. More than one because a run can be wrong in a
    /// way that depends on what ran before it, and one measurement cannot show that.
    int verify{2};
};

void say(char const* what);

/// Reads a file whole, announcing its name and size. Throws when it is not there.
std::vector<char> read_file(fs::path const& path);

float        from_bf16(std::uint16_t raw);
std::uint16_t to_bf16(float value);

/**
 * @brief Deterministic values in [-1, 1), the same on every platform.
 *
 * A GEMM's operands are megabytes and cannot ship beside the artifact, so they are made here
 * instead. splitmix64 rather than <random>, whose distributions are not specified down to the
 * bit and so differ between standard libraries -- and a reference computed from different
 * numbers than the device was given is worse than no reference.
 */
class stream_of_numbers
{
    std::uint64_t m_state;

public:
    explicit stream_of_numbers(std::uint64_t seed) : m_state(seed) {}

    float next();

    /// Fills @p into with bf16 values, which is what the array reads.
    void fill(std::span<std::uint16_t> into);
};

/**
 * @brief How far a result sits from its reference, measured every way that says something.
 *
 * Three numbers rather than one, because no single one of them settles a GEMM. Per-element
 * relative error is what you want for a normalization, where every output is the same size as
 * its input; it is useless for a dot product, whose result is a sum of signed terms far larger
 * than itself, so an output that lands near zero shows an enormous relative error while being
 * perfectly good. #relative_l2 is the one to judge that by: it weighs each error by how much of
 * the answer it is, over the whole result at once.
 */
struct deviation
{
    std::size_t worst_rel_at{};
    float       worst_rel{};

    std::size_t worst_abs_at{};
    float       worst_abs{};

    /// ||got - want|| / ||want||, over the whole result.
    double relative_l2{};

    /// Elements outside both tolerances at once.
    std::size_t outside{};

    std::size_t zeros{};
};

deviation measure(std::span<std::uint16_t const> got,
                  std::span<std::uint16_t const> want,
                  float                          rel_tol,
                  float                          abs_tol);

/// Prints every measure. Names the all-zero case, which means the kernel never wrote the buffer
/// rather than that it computed zeros.
void print(deviation const& off, std::size_t total);

/// Prints the first few of each, so a layout that is wrong end-to-end is visible at a glance.
void show_head(std::span<std::uint16_t const> got, std::span<std::uint16_t const> want);

/**
 * @brief Page-aligned host bytes to hand a kernel as an argument.
 *
 * The runtime does not allocate a layer's tensors: a graph's memory planner owns them and
 * executable::wrap_argument() only registers what it owns with the driver. This is that memory
 * planner reduced to what one test needs, and using it is what makes this test walk the same path
 * a network will.
 *
 * A std::vector is not a substitute. Nothing promises its data() sits on a page boundary, and
 * storage XRT cannot pin is refused rather than quietly copied. The allocation is rounded up to
 * whole pages for the same reason, so #size is what was pinned rather than what was asked for.
 */
class argument_storage
{
    struct release
    {
        void operator()(void* held) const noexcept;
    };

    std::unique_ptr<std::byte, release> m_held;
    std::size_t                         m_bytes{};

public:
    argument_storage() = default;

    /// @throws std::bad_alloc
    explicit argument_storage(std::size_t bytes);

    std::byte*  data() const { return m_held.get(); }
    std::size_t size() const { return m_bytes; }

    /// The bytes read back as elements, for comparing a result against a reference.
    template <typename T>
    std::span<T const> as(std::size_t count) const
    {
        return std::span{reinterpret_cast<T const*>(data()), count};
    }
};

void describe_signature(xrt::xclbin const& binary);

/// The four words every transaction stream opens with, so a malformed one is visible here rather
/// than as a hang on the device.
void describe_header(std::span<command::word const> stream);

/// Byte-compares a wired stream against what aiecc produced for the same design and shape.
bool matches_reference(std::span<command::word const> ours, fs::path const& reference);

} // namespace hw
