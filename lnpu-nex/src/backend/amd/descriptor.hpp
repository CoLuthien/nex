#pragma once

#include "design.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace lnpu::nex::amd
{

/**
 * @brief An xclbin's USER_METADATA, parsed once and read many times.
 *
 * The baking script writes a design's fixed facts in here, so the runtime never restates them.
 * This type does not interpret them. Every field is pulled by the program that knows what the
 * field means -- gemm reads its tile, rmsnorm reads whether it was baked with a weight stream --
 * which keeps operator-specific knowledge inside the operator instead of spreading it over a
 * loader that would have to know every operator to hold their fields.
 *
 * Accessors throw naming the field and the operator it belonged to. That is not defensiveness:
 * a descriptor that disagrees with the xclbin it rode in on fails nothing later. It computes a
 * wrong answer, and there is no later point at which anything notices.
 *
 * Copies share the parsed body, so passing one by value is cheap. Nothing here needs an xclbin
 * or a device -- parse() takes text -- which is what lets a design be built from a literal on a
 * machine with no NPU in it.
 */
class descriptor
{
    struct body;
    std::shared_ptr<body const> m_body;

    explicit descriptor(std::shared_ptr<body const> parsed);

public:
    /**
     * @param json the USER_METADATA payload.
     * @throws std::runtime_error when it is not json, is not an object, names no "op", or states
     *         a "schema" this build does not read.
     */
    static descriptor parse(std::string_view json);

    /// Which program reads this design. A program's describe() refuses a mismatch, and that is
    /// the one place an rmsnorm xclbin handed to a gemm gets caught.
    std::string_view op() const;

    /// @name Required fields
    /// Each throws when the field is absent or is not of the type asked for.
    /// @{
    std::uint32_t    u32(std::string_view field) const;
    std::uint32_t    u32(std::string_view group, std::string_view field) const;
    bool             flag(std::string_view field) const;
    std::string_view text(std::string_view field) const;

    /// A field written as a hex string, which is how an address is stored.
    std::uint32_t address(std::string_view field) const;
    /// @}

    /// @name Optional fields
    /// The caller states the fallback, because an absent field means something different for
    /// every operator and this has no way to know which.
    /// @{
    std::uint32_t u32_or(std::string_view field, std::uint32_t fallback) const;
    bool          flag_or(std::string_view field, bool fallback) const;
    std::uint32_t address_or(std::string_view field, std::uint32_t fallback) const;
    /// @}

    /// A list of hex strings, as parameter_slots is written. Empty when the field is absent or
    /// null, which is a design whose cores need telling nothing.
    std::vector<std::uint32_t> addresses(std::string_view field) const;

    /// Where a named kernel argument sits in "args", which is the index ddr_patch patches by and
    /// the position execute() expects that buffer in.
    /// @throws std::runtime_error when "args" does not name it.
    command::word argument(std::string_view name) const;

    /// The part every design carries, so that a program's describe() only handles its own.
    amd::design common() const;
};

} // namespace lnpu::nex::amd
