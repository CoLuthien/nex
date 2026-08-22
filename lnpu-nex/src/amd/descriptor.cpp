
#include "descriptor.hpp"

#include <nlohmann/json.hpp>

#include <stdexcept>

namespace lnpu::nex::amd
{
namespace
{

using json = ::nlohmann::json;

/// Descriptor layouts this build reads. Bumped when a field changes meaning rather than when one
/// is added, since an added field an older reader ignores costs nothing.
constexpr int kSchema = 1;

} // namespace

struct descriptor::body
{
    json             root;
    std::string      op;
    std::vector<std::string> args;
};

descriptor::descriptor(std::shared_ptr<body const> parsed) : m_body(std::move(parsed))
{
}

namespace
{

/// Every failure names the design it came out of. By the time a wrong field is noticed, the
/// xclbin it came from is several layers away and nothing else says which one it was.
std::string
prefix(std::string const& op)
{
    return "[amd::descriptor] '" + op + "' ";
}

} // namespace

descriptor
descriptor::parse(std::string_view text)
{
    auto parsed = std::make_shared<body>();

    try
    {
        parsed->root = json::parse(text);
    }
    catch (json::parse_error const& failure)
    {
        throw std::runtime_error(std::string{"[amd::descriptor] not json: "} + failure.what());
    }

    if (not parsed->root.is_object())
    {
        throw std::runtime_error("[amd::descriptor] is not an object");
    }

    auto const named = parsed->root.find("op");
    if (named == parsed->root.end() or not named->is_string())
    {
        throw std::runtime_error("[amd::descriptor] names no 'op', so nothing can read it");
    }
    parsed->op = named->get<std::string>();

    auto const schema = parsed->root.value("schema", 0);
    if (schema != kSchema)
    {
        throw std::runtime_error(prefix(parsed->op) + "is schema " + std::to_string(schema) +
                                 "; this build reads " + std::to_string(kSchema) +
                                 ". Re-bake it, or read it with the runtime it was baked for");
    }

    if (auto const listed = parsed->root.find("args"); listed != parsed->root.end())
    {
        if (not listed->is_array())
        {
            throw std::runtime_error(prefix(parsed->op) + "has an 'args' that is not a list");
        }

        for (auto const& name : *listed)
        {
            parsed->args.emplace_back(name.get<std::string>());
        }
    }

    return descriptor{std::move(parsed)};
}

std::string_view
descriptor::op() const
{
    return m_body->op;
}

namespace
{

json const&
required(json const& root, std::string const& op, std::string_view field)
{
    auto const found = root.find(field);
    if (found == root.end() or found->is_null())
    {
        throw std::runtime_error(prefix(op) + "carries no '" + std::string{field} + "'");
    }

    return *found;
}

/// Reads "0x1f000". Addresses are stored as text because a descriptor is read by people as often
/// as by programs, and a register address in decimal is read by neither.
std::uint32_t
as_address(std::string const& op, std::string_view field, json const& entry)
{
    if (not entry.is_string())
    {
        throw std::runtime_error(prefix(op) + "has a '" + std::string{field} +
                                 "' that is not a hex string");
    }

    auto const text = entry.get<std::string>();

    try
    {
        std::size_t consumed = 0;
        auto const  value    = std::stoul(text, &consumed, 16);
        if (consumed != text.size()) throw std::invalid_argument{text};

        return static_cast<std::uint32_t>(value);
    }
    catch (std::exception const&)
    {
        throw std::runtime_error(prefix(op) + "has '" + std::string{field} + "' = \"" + text +
                                 "\", which is not an address");
    }
}

} // namespace

std::uint32_t
descriptor::u32(std::string_view field) const
{
    auto const& entry = required(m_body->root, m_body->op, field);
    if (not entry.is_number_unsigned())
    {
        throw std::runtime_error(prefix(m_body->op) + "has a '" + std::string{field} +
                                 "' that is not a whole number");
    }

    return entry.get<std::uint32_t>();
}

std::uint32_t
descriptor::u32(std::string_view group, std::string_view field) const
{
    auto const& nested = required(m_body->root, m_body->op, group);
    if (not nested.is_object())
    {
        throw std::runtime_error(prefix(m_body->op) + "has a '" + std::string{group} +
                                 "' that is not a group");
    }

    auto const found = nested.find(field);
    if (found == nested.end() or not found->is_number_unsigned())
    {
        throw std::runtime_error(prefix(m_body->op) + "carries no whole number '" +
                                 std::string{group} + "." + std::string{field} + "'");
    }

    return found->get<std::uint32_t>();
}

bool
descriptor::flag(std::string_view field) const
{
    auto const& entry = required(m_body->root, m_body->op, field);
    if (not entry.is_boolean())
    {
        throw std::runtime_error(prefix(m_body->op) + "has a '" + std::string{field} +
                                 "' that is not true or false");
    }

    return entry.get<bool>();
}

std::string_view
descriptor::text(std::string_view field) const
{
    auto const& entry = required(m_body->root, m_body->op, field);
    if (not entry.is_string())
    {
        throw std::runtime_error(prefix(m_body->op) + "has a '" + std::string{field} +
                                 "' that is not text");
    }

    return entry.get_ref<std::string const&>();
}

std::uint32_t
descriptor::address(std::string_view field) const
{
    return as_address(m_body->op, field, required(m_body->root, m_body->op, field));
}

std::uint32_t
descriptor::u32_or(std::string_view field, std::uint32_t fallback) const
{
    auto const found = m_body->root.find(field);
    return found == m_body->root.end() or found->is_null() ? fallback : u32(field);
}

bool
descriptor::flag_or(std::string_view field, bool fallback) const
{
    auto const found = m_body->root.find(field);
    return found == m_body->root.end() or found->is_null() ? fallback : flag(field);
}

std::uint32_t
descriptor::address_or(std::string_view field, std::uint32_t fallback) const
{
    auto const found = m_body->root.find(field);
    return found == m_body->root.end() or found->is_null() ? fallback : address(field);
}

std::vector<std::uint32_t>
descriptor::addresses(std::string_view field) const
{
    auto const found = m_body->root.find(field);
    if (found == m_body->root.end() or found->is_null())
    {
        return {};
    }

    if (not found->is_array())
    {
        throw std::runtime_error(prefix(m_body->op) + "has a '" + std::string{field} +
                                 "' that is not a list of addresses");
    }

    std::vector<std::uint32_t> listed{};
    listed.reserve(found->size());
    for (auto const& entry : *found)
    {
        listed.emplace_back(as_address(m_body->op, field, entry));
    }

    return listed;
}

command::word
descriptor::argument(std::string_view name) const
{
    for (std::size_t at = 0; at < m_body->args.size(); ++at)
    {
        if (m_body->args[at] == name)
        {
            return static_cast<command::word>(at);
        }
    }

    std::string held{};
    for (auto const& argument : m_body->args)
    {
        held += held.empty() ? "" : ", ";
        held += argument;
    }

    throw std::runtime_error(prefix(m_body->op) + "takes no argument '" + std::string{name} +
                             "'; it takes " + (held.empty() ? "none" : held));
}

amd::design
descriptor::common() const
{
    auto const named = text("generation");

    npu::generation generation{};
    if (named == "npu2")
    {
        generation = npu::npu2;
    }
    else if (named == "npu1")
    {
        generation = npu::npu1;
    }
    else
    {
        throw std::runtime_error(prefix(m_body->op) + "was baked for '" + std::string{named} +
                                 "', which this build does not emit for");
    }

    return amd::design{
        .generation        = generation,
        .partition_columns = u32("partition_columns"),
        .columns           = u32("columns"),
        .parameter_slots   = addresses("parameter_slots"),
        .start_register    = address_or("start_register", 0),
    };
}

} // namespace lnpu::nex::amd
