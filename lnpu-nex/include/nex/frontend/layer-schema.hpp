
#pragma once

#include "nex-api.hpp"
#include "nex/nex-types.hpp"

#include "cmn/numeric-types.hpp"

#include <optional>
#include <string>
#include <vector>

namespace lnpu::nex
{

enum class EArity
{
    single,
    optional,
    variadic, // placeholder for future
};

enum class EAttributeType
{
    f32,
    i64,
    str,
    f32s,
    i64s,
    strs,
    tensor
};

struct type_constraint
{                                // ONNX의 "T": {float,double,...} 개념
    std::string            name; // "T"
    std::vector<EDataType> allowed;
};

struct operand_role
{
    std::string name;
    std::string type_param; // references type_constraint.name; empty = any
    EArity      arity = EArity::single;
};

using result_role = operand_role;

struct attribute_role
{
    bool                           required = false;
    std::string                    name;
    EAttributeType                 type;
    std::optional<attribute_value> default_value;
};

struct layer_schema
{
    std::string domain;
    std::string optype;

    std::vector<operand_role>    operands;
    std::vector<result_role>     results;
    std::vector<type_constraint> type_constraints;

    std::vector<attribute_role> attrs;
};
} // namespace lnpu::nex
