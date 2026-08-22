
#include "schema-registry.hpp"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <functional>
#include <iterator>
#include <mutex>
#include <string>
#include <type_traits>
#include <utility>

namespace lnpu::nex::nx
{
namespace
{

std::string
key_of(std::string_view domain, std::string_view optype)
{
    std::string key{};
    key.reserve(domain.size() + optype.size() + 1);

    key.append(domain);
    key.push_back(':');
    key.append(optype);

    return key;
}

/// Orders and searches the version list by the opset an entry became effective in.
constexpr auto by_since_version = [](auto const& left, auto const& right) {
    if constexpr (std::is_integral_v<std::decay_t<decltype(left)>>)
    {
        return left < right.since_version;
    }
    else
    {
        return left.since_version < right;
    }
};

} // namespace

void
schema_registry::register_schema(std::int64_t since_version, layer_schema schema)
{
    auto& versions = m_ops[key_of(schema.domain, schema.optype)];

    auto const at =
        std::lower_bound(versions.begin(), versions.end(), since_version, by_since_version);

    if (at != versions.end() and at->since_version == since_version)
    {
        at->schema = std::move(schema);
        return;
    }

    versions.insert(at, versioned_schema{since_version, std::move(schema)});
}

layer_schema const*
schema_registry::find(std::string_view optype, std::string_view domain, std::int64_t opset) const
{
    auto const entry = m_ops.find(key_of(domain, optype));
    if (entry == m_ops.end())
    {
        if (auto const* reason = onnx_schema_exclusion(optype, domain))
        {
            spdlog::warn("[nx] '{}' is known but carries no signature here ({})", optype, reason);
        }
        else
        {
            spdlog::warn(
                "[nx] '{}' (domain '{}') is not an operator this build knows", optype, domain);
        }

        return nullptr;
    }

    auto const& versions = entry->second; // ascending by since_version

    // the entry that governs `opset` is the last one that became effective no later than it
    auto const beyond = std::upper_bound(versions.begin(), versions.end(), opset, by_since_version);

    if (beyond == versions.begin())
    {
        spdlog::warn("[nx] '{}' exists only from opset {} on, but the model asks for opset {}",
                     optype,
                     versions.front().since_version,
                     opset);

        return nullptr;
    }

    return &std::prev(beyond)->schema;
}

schema_registry&
op_schema_registry()
{
    // Function-local so that the table is built the first time somebody asks for it rather than
    // during static initialisation, and once even if two threads ask at the same time.
    static schema_registry registry{};
    static std::once_flag  filled{};

    std::call_once(filled, register_onnx_schemas, std::ref(registry));

    return registry;
}

} // namespace lnpu::nex::nx
