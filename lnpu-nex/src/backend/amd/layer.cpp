
#include "layer.hpp"

#include <spdlog/spdlog.h>

#include <stdexcept>
#include <utility>

namespace lnpu::nex::amd
{
namespace
{

std::error_code
refuse(std::string_view tensor, std::string const& why)
{
    spdlog::error("[nex::amd::layer] '{}' {}", tensor, why);
    return std::make_error_code(std::errc::invalid_argument);
}

/// Whether a buffer is the one the graph said would be here. An optional the graph left unset is
/// something it did not promise, so nothing is checked against it.
bool
matches(value_description const& expected, encoded const& target)
{
    if (expected.dtype and *expected.dtype != target.dtype()) return false;
    if (expected.layout and *expected.layout != target.layout()) return false;

    return true;
}

} // namespace

layer::unique
layer::create(layer_description const& description, operation::executable::unique exec)
{
    layer::unique result{};
    try
    {
        result = std::make_unique<layer>(description, std::move(exec));
    }
    catch (std::exception const& e)
    {
        spdlog::error(e.what());
    }

    return result;
}

layer::layer(layer_description const& description, operation::executable::unique executable)
    : m_executable(std::move(executable))
{
    if (nullptr == m_executable)
    {
        throw std::invalid_argument("[nex::amd::layer] there is no executable to drive");
    }

    auto const bindings = m_executable->bindings();
    if (bindings.empty())
    {
        throw std::invalid_argument(
            "[nex::amd::layer] this executable was built by hand and names no tensors, so there "
            "is nothing to bind by name; run it positionally instead");
    }

    for (auto const& what : bindings)
    {
        if (what.tensor.empty())
        {
            throw std::invalid_argument("[nex::amd::layer] an operand reached this without a name");
        }

        // Which way the bytes move is the graph's word, not the program's -- so it is read off
        // the description rather than carried through the binding.
        auto const* produced = description.output(what.tensor);
        auto const* consumed = description.input(what.tensor);
        if (nullptr == produced and nullptr == consumed)
        {
            throw std::invalid_argument("[nex::amd::layer] '" + std::string{description.name()} +
                                        "' has no tensor called '" + what.tensor + "'");
        }

        auto const argument = static_cast<std::size_t>(what.argument_index);
        if (argument >= m_arguments.size())
        {
            m_arguments.resize(argument + 1);
        }

        auto const placed = m_slots.emplace(what.tensor,
                                            slot{
                                                .expected = *(produced ? produced : consumed),
                                                .argument = argument,
                                                .produced = produced != nullptr,
                                            });

        if (not placed.second)
        {
            throw std::invalid_argument("[nex::amd::layer] '" + what.tensor +
                                        "' is named by more than one argument");
        }
    }
}

bool
layer::takes(std::string_view key) const
{
    return m_slots.contains(key);
}

std::error_code
layer::execute()
{
    // An unbound argument would be an empty buffer object, which the driver resolves to an
    // address of zero and the array then reads. Counting is enough to catch it because a slot is
    // only ever bound or cleared through bind()/reset().
    if (m_bound != m_slots.size())
    {
        spdlog::error("[nex::amd::layer] {} of {} tensors are bound", m_bound, m_slots.size());
        return std::make_error_code(std::errc::no_such_device_or_address);
    }

    return m_executable->execute(m_arguments);
}

std::error_code
layer::set_input(std::string_view key, encoded::shared target)
{
    return bind(key, false, std::move(target));
}

std::error_code
layer::set_output(std::string_view key, encoded::shared target)
{
    return bind(key, true, std::move(target));
}

std::error_code
layer::reset_input(std::string_view key)
{
    return reset(key, false);
}

std::error_code
layer::reset_output(std::string_view key)
{
    return reset(key, true);
}

std::error_code
layer::bind(std::string_view key, bool produced, encoded::shared target)
{
    auto const found = m_slots.find(key);
    if (found == m_slots.end())
    {
        return refuse(key, "is not a tensor this layer takes");
    }

    auto& held = found->second;
    if (held.produced != produced)
    {
        return refuse(key,
                      held.produced ? "is a result, and was bound as an operand"
                                    : "is an operand, and was bound as a result");
    }

    if (nullptr == target)
    {
        return refuse(key, "cannot be bound to nothing; reset it instead");
    }

    if (not matches(held.expected, *target))
    {
        return refuse(key, "was given a buffer whose type or layout is not what the graph states");
    }

    xrt::bo wrapped{};
    try
    {
        wrapped = m_executable->wrap_argument(target->ptr(), target->n_bytes());
    }
    catch (std::exception const& thrown)
    {
        // XRT refuses storage it cannot pin, and that is about the buffer the caller passed --
        // an ordinary answer while wiring up a graph, not a programming error.
        return refuse(key, std::string{"cannot be used as a kernel argument: "} + thrown.what());
    }

    // Rebinding a bound tensor replaces it and is not a second binding.
    if (nullptr == held.target) ++m_bound;

    m_arguments[held.argument] = std::move(wrapped);
    held.target                = std::move(target);

    return {};
}

std::error_code
layer::reset(std::string_view key, bool produced)
{
    auto const found = m_slots.find(key);
    if (found == m_slots.end())
    {
        return refuse(key, "is not a tensor this layer takes");
    }

    auto& held = found->second;
    if (held.produced != produced)
    {
        return refuse(key,
                      held.produced ? "is a result, and was reset as an operand"
                                    : "is an operand, and was reset as a result");
    }

    if (nullptr == held.target) return {};

    // The view goes before the storage it names, not after.
    m_arguments[held.argument] = {};
    held.target.reset();
    --m_bound;

    return {};
}

} // namespace lnpu::nex::amd
