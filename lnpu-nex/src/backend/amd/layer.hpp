
#pragma once

#include "backend/layer.hpp"

#include "amd/operation.hpp"

#include "nex/frontend/layer-description.hpp"

#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

namespace lnpu::nex::amd
{

/**
 * @brief One graph layer, driven by name, over one reusable executable.
 *
 * The executable knows which argument slot each of its operands occupies and what the graph calls
 * them, because its program's lower() is where both were decided. This turns that into the thing
 * a caller wants: set_input("attn.q.weight", buffer). Nothing here knows what a gemm is, and the
 * lookup is a map built once rather than a search per bind.
 *
 * Storage is never allocated here. A graph's memory planner owns the bytes and this only registers
 * them with the driver, which is why every bound tensor is held: the view the driver resolves
 * addresses from must not outlive what it names.
 */
class layer final : public nex::layer
{
    struct slot
    {
        /// What the graph promised about this tensor. Its optionals are checked when set, and an
        /// absent one is a graph that did not say, not a mismatch.
        value_description expected;

        /// Index into #m_arguments, which is how descriptor::argument() counts.
        std::size_t argument{};

        /// Whether the graph calls this a result. set_input() on a result is refused rather than
        /// quietly bound, since the two differ only in which way the bytes move.
        bool produced{};

        encoded::shared target;
    };

    operation::executable::unique m_executable;

    /// Heterogeneous compare so a string_view key is looked up without allocating one.
    std::map<std::string, slot, std::less<>> m_slots;

    /// Handed to the executable as-is. An entry is default-constructed until its tensor is bound.
    std::vector<xrt::bo> m_arguments;
    std::size_t          m_bound{};

public:
    using unique = std::unique_ptr<layer>;

    /**
     * @brief Pairs a lowered executable with the description it was lowered from.
     *
     * @throws std::invalid_argument when the two disagree -- a null executable, one built by hand
     *         and so carrying no names, a name the description does not have, or one argument
     *         claimed twice. Every one of them is a wiring mistake between the program and the
     *         graph rather than anything a caller did, so none of them is an error code.
     */
    layer(layer_description const& description, operation::executable::unique executable);
    ~layer() override = default;

    std::error_code execute() override;

    std::error_code set_input(std::string_view key, encoded::shared target) override;
    std::error_code set_output(std::string_view key, encoded::shared target) override;

    std::error_code reset_input(std::string_view key) override;
    std::error_code reset_output(std::string_view key) override;

private:
    std::error_code bind(std::string_view key, bool produced, encoded::shared target);
    std::error_code reset(std::string_view key, bool produced);
};

} // namespace lnpu::nex::amd
