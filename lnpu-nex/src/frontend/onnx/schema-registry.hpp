
#pragma once

#include "nex-api.hpp"

#include "nex/frontend/layer-schema.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace lnpu::nex::nx
{

class NEX_EXPORT schema_registry
{
public:
    // Registers `schema` as effective from opset `since_version` (inclusive) for its
    // (domain, optype). Re-registering the same since_version overwrites.
    void register_schema(std::int64_t since_version, layer_schema schema);

    // Resolves the schema governing an op at `opset`: the registered entry for
    // (domain, optype) with the greatest since_version <= opset. Returns nullptr if the
    // op is unknown or predates every registered version.
    layer_schema const* find(std::string_view optype,
                             std::string_view domain,
                             std::int64_t     opset) const;

private:
    struct versioned_schema
    {
        std::int64_t since_version;
        layer_schema schema;
    };

    // key = domain + "." + optype ; value kept sorted by since_version ascending
    std::unordered_map<std::string, std::vector<versioned_schema>> m_ops;
};

// Process-wide registry. Operator schemas are global static knowledge, and it must
// outlive every layer that caches a `layer_schema const*` from it.
schema_registry& op_schema_registry();

// Both live in op-schemas.gen.cpp, which generate-op-schemas.py writes from the onnx package.

// Fills `registry` with every operator version this build carries.
void register_onnx_schemas(schema_registry& registry);

// Why an operator onnx defines was left out of that table, or nullptr when it was not left out
// (it may still be unknown -- onnx has operators this build predates).
char const* onnx_schema_exclusion(std::string_view optype, std::string_view domain);

} // namespace lnpu::nex::nx
