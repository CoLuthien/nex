// Written by generate-op-schemas.py. Regenerate rather than edit.
//
// onnx package : 1.22.0
// newest opset : 27
// domains      : "" (ai.onnx)
//
// What this table leaves out, and why, is listed in op-schemas-coverage.md.

#include "schema-registry.hpp"

#include <cstdint>
#include <string_view>

namespace lnpu::nex::nx
{

static void
register_Abs(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Abs",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Abs",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Abs",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Acos(schema_registry& r)
{
    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Acos",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Acos",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Acosh(schema_registry& r)
{
    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "Acosh",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Acosh",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Add(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Add",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                              {.required = false, .name = "broadcast", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Add",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                              {.required = false, .name = "broadcast", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Add",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Add",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(14,
                      layer_schema{
                          .domain = "",
                          .optype = "Add",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_AffineGrid(schema_registry& r)
{
    r.register_schema(20,
                      layer_schema{
                          .domain = "",
                          .optype = "AffineGrid",
                          .operands = {
                              {.name = "theta", .type_param = "T1"},
                              {.name = "size", .type_param = "T2"},
                          },
                          .results = {
                              {.name = "grid", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "align_corners",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_And(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "And",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::u8}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                              {.required = false, .name = "broadcast", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "And",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::u8}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });
}

static void
register_ArgMax(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "ArgMax",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "tensor(int64)"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "ArgMax",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "tensor(int64)"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(12,
                      layer_schema{
                          .domain = "",
                          .optype = "ArgMax",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "tensor(int64)"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "select_last_index",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "ArgMax",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "tensor(int64)"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "select_last_index",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_ArgMin(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "ArgMin",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "tensor(int64)"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "ArgMin",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "tensor(int64)"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(12,
                      layer_schema{
                          .domain = "",
                          .optype = "ArgMin",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "tensor(int64)"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "select_last_index",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "ArgMin",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "tensor(int64)"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "select_last_index",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_Asin(schema_registry& r)
{
    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Asin",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Asin",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Asinh(schema_registry& r)
{
    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "Asinh",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Asinh",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Atan(schema_registry& r)
{
    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Atan",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Atan",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Atanh(schema_registry& r)
{
    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "Atanh",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Atanh",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Attention(schema_registry& r)
{
    r.register_schema(23,
                      layer_schema{
                          .domain = "",
                          .optype = "Attention",
                          .operands = {
                              {.name = "Q", .type_param = "T1"},
                              {.name = "K", .type_param = "T1"},
                              {.name = "V", .type_param = "T2"},
                              {.name = "attn_mask", .type_param = "U", .arity = EArity::optional},
                              {.name = "past_key", .type_param = "T1", .arity = EArity::optional},
                              {.name = "past_value", .type_param = "T2", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T1"},
                              {.name = "present_key", .type_param = "T1",
                               .arity = EArity::optional},
                              {.name = "present_value", .type_param = "T2",
                               .arity = EArity::optional},
                              {.name = "qk_matmul_output", .type_param = "T1",
                               .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "U", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "is_causal", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "kv_num_heads",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "q_num_heads",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "qk_matmul_output_mode",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "scale", .type = EAttributeType::f32},
                              {.required = false, .name = "softcap", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.0f}},
                              {.required = false, .name = "softmax_precision",
                               .type = EAttributeType::i64},
                          },
                      });

    r.register_schema(24,
                      layer_schema{
                          .domain = "",
                          .optype = "Attention",
                          .operands = {
                              {.name = "Q", .type_param = "T1"},
                              {.name = "K", .type_param = "T1"},
                              {.name = "V", .type_param = "T2"},
                              {.name = "attn_mask", .type_param = "U", .arity = EArity::optional},
                              {.name = "past_key", .type_param = "T1", .arity = EArity::optional},
                              {.name = "past_value", .type_param = "T2", .arity = EArity::optional},
                              {.name = "nonpad_kv_seqlen", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T1"},
                              {.name = "present_key", .type_param = "T1",
                               .arity = EArity::optional},
                              {.name = "present_value", .type_param = "T2",
                               .arity = EArity::optional},
                              {.name = "qk_matmul_output", .type_param = "T1",
                               .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "U", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "is_causal", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "kv_num_heads",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "q_num_heads",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "qk_matmul_output_mode",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "scale", .type = EAttributeType::f32},
                              {.required = false, .name = "softcap", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.0f}},
                              {.required = false, .name = "softmax_precision",
                               .type = EAttributeType::i64},
                          },
                      });
}

static void
register_AveragePool(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "AveragePool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = true, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "AveragePool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "count_include_pad",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = true, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(10,
                      layer_schema{
                          .domain = "",
                          .optype = "AveragePool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "ceil_mode", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "count_include_pad",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = true, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "AveragePool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "ceil_mode", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "count_include_pad",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = true, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(19,
                      layer_schema{
                          .domain = "",
                          .optype = "AveragePool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "ceil_mode", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "count_include_pad",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "dilations",
                               .type = EAttributeType::i64s},
                              {.required = true, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "AveragePool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "ceil_mode", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "count_include_pad",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "dilations",
                               .type = EAttributeType::i64s},
                              {.required = true, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });
}

static void
register_BatchNormalization(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "BatchNormalization",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "scale", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                              {.name = "mean", .type_param = "T"},
                              {.name = "var", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                              {.name = "mean", .type_param = "T", .arity = EArity::optional},
                              {.name = "var", .type_param = "T", .arity = EArity::optional},
                              {.name = "saved_mean", .type_param = "T", .arity = EArity::optional},
                              {.name = "saved_var", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "epsilon", .type = EAttributeType::f32,
                               .default_value = attribute_value{9.999999747378752e-06f}},
                              {.required = false, .name = "is_test", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "momentum", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.8999999761581421f}},
                              {.required = false, .name = "spatial", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "BatchNormalization",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "scale", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                              {.name = "mean", .type_param = "T"},
                              {.name = "var", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                              {.name = "mean", .type_param = "T", .arity = EArity::optional},
                              {.name = "var", .type_param = "T", .arity = EArity::optional},
                              {.name = "saved_mean", .type_param = "T", .arity = EArity::optional},
                              {.name = "saved_var", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "epsilon", .type = EAttributeType::f32,
                               .default_value = attribute_value{9.999999747378752e-06f}},
                              {.required = false, .name = "is_test", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "momentum", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.8999999761581421f}},
                              {.required = false, .name = "spatial", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "BatchNormalization",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "scale", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                              {.name = "mean", .type_param = "T"},
                              {.name = "var", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                              {.name = "mean", .type_param = "T", .arity = EArity::optional},
                              {.name = "var", .type_param = "T", .arity = EArity::optional},
                              {.name = "saved_mean", .type_param = "T", .arity = EArity::optional},
                              {.name = "saved_var", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "epsilon", .type = EAttributeType::f32,
                               .default_value = attribute_value{9.999999747378752e-06f}},
                              {.required = false, .name = "momentum", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.8999999761581421f}},
                              {.required = false, .name = "spatial", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "BatchNormalization",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "scale", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                              {.name = "mean", .type_param = "T"},
                              {.name = "var", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                              {.name = "mean", .type_param = "T", .arity = EArity::optional},
                              {.name = "var", .type_param = "T", .arity = EArity::optional},
                              {.name = "saved_mean", .type_param = "T", .arity = EArity::optional},
                              {.name = "saved_var", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "epsilon", .type = EAttributeType::f32,
                               .default_value = attribute_value{9.999999747378752e-06f}},
                              {.required = false, .name = "momentum", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.8999999761581421f}},
                          },
                      });

    r.register_schema(14,
                      layer_schema{
                          .domain = "",
                          .optype = "BatchNormalization",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "scale", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                              {.name = "input_mean", .type_param = "U"},
                              {.name = "input_var", .type_param = "U"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                              {.name = "running_mean", .type_param = "U",
                               .arity = EArity::optional},
                              {.name = "running_var", .type_param = "U", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                              {.name = "U", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "epsilon", .type = EAttributeType::f32,
                               .default_value = attribute_value{9.999999747378752e-06f}},
                              {.required = false, .name = "momentum", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.8999999761581421f}},
                              {.required = false, .name = "training_mode",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(15,
                      layer_schema{
                          .domain = "",
                          .optype = "BatchNormalization",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "scale", .type_param = "T1"},
                              {.name = "B", .type_param = "T1"},
                              {.name = "input_mean", .type_param = "T2"},
                              {.name = "input_var", .type_param = "T2"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                              {.name = "running_mean", .type_param = "T2",
                               .arity = EArity::optional},
                              {.name = "running_var", .type_param = "T2",
                               .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "epsilon", .type = EAttributeType::f32,
                               .default_value = attribute_value{9.999999747378752e-06f}},
                              {.required = false, .name = "momentum", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.8999999761581421f}},
                              {.required = false, .name = "training_mode",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_Bernoulli(schema_registry& r)
{
    r.register_schema(15,
                      layer_schema{
                          .domain = "",
                          .optype = "Bernoulli",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "dtype", .type = EAttributeType::i64},
                              {.required = false, .name = "seed", .type = EAttributeType::f32},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Bernoulli",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "dtype", .type = EAttributeType::i64},
                              {.required = false, .name = "seed", .type = EAttributeType::f32},
                          },
                      });
}

static void
register_BitCast(schema_registry& r)
{
    r.register_schema(26,
                      layer_schema{
                          .domain = "",
                          .optype = "BitCast",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "to", .type = EAttributeType::i64},
                          },
                      });
}

static void
register_BitShift(schema_registry& r)
{
    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "BitShift",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "Y", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Z", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::u8, EDataType::u16,
                                                        EDataType::i32}},
                          },
                          .attrs = {
                              {.required = true, .name = "direction", .type = EAttributeType::str},
                          },
                      });
}

static void
register_BitwiseAnd(schema_registry& r)
{
    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "BitwiseAnd",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32}},
                          },
                          .attrs = {},
                      });
}

static void
register_BitwiseNot(schema_registry& r)
{
    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "BitwiseNot",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32}},
                          },
                          .attrs = {},
                      });
}

static void
register_BitwiseOr(schema_registry& r)
{
    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "BitwiseOr",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32}},
                          },
                          .attrs = {},
                      });
}

static void
register_BitwiseXor(schema_registry& r)
{
    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "BitwiseXor",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32}},
                          },
                          .attrs = {},
                      });
}

static void
register_BlackmanWindow(schema_registry& r)
{
    r.register_schema(17,
                      layer_schema{
                          .domain = "",
                          .optype = "BlackmanWindow",
                          .operands = {
                              {.name = "size", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "output_datatype",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "periodic", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });
}

static void
register_Cast(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Cast",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "to", .type = EAttributeType::str},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Cast",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "to", .type = EAttributeType::i64},
                          },
                      });

    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "Cast",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "to", .type = EAttributeType::i64},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Cast",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "to", .type = EAttributeType::i64},
                          },
                      });

    r.register_schema(19,
                      layer_schema{
                          .domain = "",
                          .optype = "Cast",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "saturate", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = true, .name = "to", .type = EAttributeType::i64},
                          },
                      });

    r.register_schema(21,
                      layer_schema{
                          .domain = "",
                          .optype = "Cast",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "saturate", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = true, .name = "to", .type = EAttributeType::i64},
                          },
                      });

    r.register_schema(23,
                      layer_schema{
                          .domain = "",
                          .optype = "Cast",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "saturate", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = true, .name = "to", .type = EAttributeType::i64},
                          },
                      });

    r.register_schema(24,
                      layer_schema{
                          .domain = "",
                          .optype = "Cast",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "round_mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"up"}}},
                              {.required = false, .name = "saturate", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = true, .name = "to", .type = EAttributeType::i64},
                          },
                      });

    r.register_schema(25,
                      layer_schema{
                          .domain = "",
                          .optype = "Cast",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "round_mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"up"}}},
                              {.required = false, .name = "saturate", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = true, .name = "to", .type = EAttributeType::i64},
                          },
                      });
}

static void
register_CastLike(schema_registry& r)
{
    r.register_schema(15,
                      layer_schema{
                          .domain = "",
                          .optype = "CastLike",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                              {.name = "target_type", .type_param = "T2"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(19,
                      layer_schema{
                          .domain = "",
                          .optype = "CastLike",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                              {.name = "target_type", .type_param = "T2"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "saturate", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(21,
                      layer_schema{
                          .domain = "",
                          .optype = "CastLike",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                              {.name = "target_type", .type_param = "T2"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "saturate", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(23,
                      layer_schema{
                          .domain = "",
                          .optype = "CastLike",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                              {.name = "target_type", .type_param = "T2"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "saturate", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(24,
                      layer_schema{
                          .domain = "",
                          .optype = "CastLike",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                              {.name = "target_type", .type_param = "T2"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "round_mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"up"}}},
                              {.required = false, .name = "saturate", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(25,
                      layer_schema{
                          .domain = "",
                          .optype = "CastLike",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                              {.name = "target_type", .type_param = "T2"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "round_mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"up"}}},
                              {.required = false, .name = "saturate", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });
}

static void
register_CausalConvWithState(schema_registry& r)
{
    r.register_schema(27,
                      layer_schema{
                          .domain = "",
                          .optype = "CausalConvWithState",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "weight", .type_param = "T"},
                              {.name = "bias", .type_param = "T", .arity = EArity::optional},
                              {.name = "past_state", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                              {.name = "present_state", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "activation", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"none"}}},
                          },
                      });
}

static void
register_Ceil(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Ceil",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Ceil",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Ceil",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Celu(schema_registry& r)
{
    r.register_schema(12,
                      layer_schema{
                          .domain = "",
                          .optype = "Celu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                          },
                      });
}

static void
register_CenterCropPad(schema_registry& r)
{
    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "CenterCropPad",
                          .operands = {
                              {.name = "input_data", .type_param = "T"},
                              {.name = "shape", .type_param = "Tind"},
                          },
                          .results = {
                              {.name = "output_data", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                          },
                      });
}

static void
register_Clip(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Clip",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "max", .type = EAttributeType::f32},
                              {.required = false, .name = "min", .type = EAttributeType::f32},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Clip",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "max", .type = EAttributeType::f32,
                               .default_value = attribute_value{3.4028234663852886e+38f}},
                              {.required = false, .name = "min", .type = EAttributeType::f32,
                               .default_value = attribute_value{-3.4028234663852886e+38f}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Clip",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "min", .type_param = "T", .arity = EArity::optional},
                              {.name = "max", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(12,
                      layer_schema{
                          .domain = "",
                          .optype = "Clip",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "min", .type_param = "T", .arity = EArity::optional},
                              {.name = "max", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Clip",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "min", .type_param = "T", .arity = EArity::optional},
                              {.name = "max", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Col2Im(schema_registry& r)
{
    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "Col2Im",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "image_shape", .type_param = "tensor(int64)"},
                              {.name = "block_shape", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "dilations",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });
}

static void
register_Compress(schema_registry& r)
{
    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "Compress",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "condition", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Compress",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "condition", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                          },
                      });
}

static void
register_Concat(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Concat",
                          .operands = {
                              {.name = "inputs", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "concat_result", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                          },
                      });

    r.register_schema(4,
                      layer_schema{
                          .domain = "",
                          .optype = "Concat",
                          .operands = {
                              {.name = "inputs", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "concat_result", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "axis", .type = EAttributeType::i64},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Concat",
                          .operands = {
                              {.name = "inputs", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "concat_result", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "axis", .type = EAttributeType::i64},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Concat",
                          .operands = {
                              {.name = "inputs", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "concat_result", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "axis", .type = EAttributeType::i64},
                          },
                      });
}

static void
register_Constant(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Constant",
                          .operands = {},
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "value", .type = EAttributeType::tensor},
                          },
                      });

    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "Constant",
                          .operands = {},
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "value", .type = EAttributeType::tensor},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Constant",
                          .operands = {},
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "value", .type = EAttributeType::tensor},
                          },
                      });

    r.register_schema(12,
                      layer_schema{
                          .domain = "",
                          .optype = "Constant",
                          .operands = {},
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "value", .type = EAttributeType::tensor},
                              {.required = false, .name = "value_float",
                               .type = EAttributeType::f32},
                              {.required = false, .name = "value_floats",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "value_int", .type = EAttributeType::i64},
                              {.required = false, .name = "value_ints",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "value_string",
                               .type = EAttributeType::str},
                              {.required = false, .name = "value_strings",
                               .type = EAttributeType::strs},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Constant",
                          .operands = {},
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "value", .type = EAttributeType::tensor},
                              {.required = false, .name = "value_float",
                               .type = EAttributeType::f32},
                              {.required = false, .name = "value_floats",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "value_int", .type = EAttributeType::i64},
                              {.required = false, .name = "value_ints",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "value_string",
                               .type = EAttributeType::str},
                              {.required = false, .name = "value_strings",
                               .type = EAttributeType::strs},
                          },
                      });

    r.register_schema(19,
                      layer_schema{
                          .domain = "",
                          .optype = "Constant",
                          .operands = {},
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "value", .type = EAttributeType::tensor},
                              {.required = false, .name = "value_float",
                               .type = EAttributeType::f32},
                              {.required = false, .name = "value_floats",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "value_int", .type = EAttributeType::i64},
                              {.required = false, .name = "value_ints",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "value_string",
                               .type = EAttributeType::str},
                              {.required = false, .name = "value_strings",
                               .type = EAttributeType::strs},
                          },
                      });

    r.register_schema(21,
                      layer_schema{
                          .domain = "",
                          .optype = "Constant",
                          .operands = {},
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "value", .type = EAttributeType::tensor},
                              {.required = false, .name = "value_float",
                               .type = EAttributeType::f32},
                              {.required = false, .name = "value_floats",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "value_int", .type = EAttributeType::i64},
                              {.required = false, .name = "value_ints",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "value_string",
                               .type = EAttributeType::str},
                              {.required = false, .name = "value_strings",
                               .type = EAttributeType::strs},
                          },
                      });

    r.register_schema(23,
                      layer_schema{
                          .domain = "",
                          .optype = "Constant",
                          .operands = {},
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "value", .type = EAttributeType::tensor},
                              {.required = false, .name = "value_float",
                               .type = EAttributeType::f32},
                              {.required = false, .name = "value_floats",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "value_int", .type = EAttributeType::i64},
                              {.required = false, .name = "value_ints",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "value_string",
                               .type = EAttributeType::str},
                              {.required = false, .name = "value_strings",
                               .type = EAttributeType::strs},
                          },
                      });

    r.register_schema(24,
                      layer_schema{
                          .domain = "",
                          .optype = "Constant",
                          .operands = {},
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "value", .type = EAttributeType::tensor},
                              {.required = false, .name = "value_float",
                               .type = EAttributeType::f32},
                              {.required = false, .name = "value_floats",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "value_int", .type = EAttributeType::i64},
                              {.required = false, .name = "value_ints",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "value_string",
                               .type = EAttributeType::str},
                              {.required = false, .name = "value_strings",
                               .type = EAttributeType::strs},
                          },
                      });

    r.register_schema(25,
                      layer_schema{
                          .domain = "",
                          .optype = "Constant",
                          .operands = {},
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "value", .type = EAttributeType::tensor},
                              {.required = false, .name = "value_float",
                               .type = EAttributeType::f32},
                              {.required = false, .name = "value_floats",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "value_int", .type = EAttributeType::i64},
                              {.required = false, .name = "value_ints",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "value_string",
                               .type = EAttributeType::str},
                              {.required = false, .name = "value_strings",
                               .type = EAttributeType::strs},
                          },
                      });
}

static void
register_ConstantOfShape(schema_registry& r)
{
    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "ConstantOfShape",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "value", .type = EAttributeType::tensor},
                          },
                      });

    r.register_schema(20,
                      layer_schema{
                          .domain = "",
                          .optype = "ConstantOfShape",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "value", .type = EAttributeType::tensor},
                          },
                      });

    r.register_schema(21,
                      layer_schema{
                          .domain = "",
                          .optype = "ConstantOfShape",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "value", .type = EAttributeType::tensor},
                          },
                      });

    r.register_schema(23,
                      layer_schema{
                          .domain = "",
                          .optype = "ConstantOfShape",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "value", .type = EAttributeType::tensor},
                          },
                      });

    r.register_schema(24,
                      layer_schema{
                          .domain = "",
                          .optype = "ConstantOfShape",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "value", .type = EAttributeType::tensor},
                          },
                      });

    r.register_schema(25,
                      layer_schema{
                          .domain = "",
                          .optype = "ConstantOfShape",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "value", .type = EAttributeType::tensor},
                          },
                      });
}

static void
register_Conv(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Conv",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "dilations",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "group", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Conv",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "dilations",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "group", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Conv",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "dilations",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "group", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });
}

static void
register_ConvInteger(schema_registry& r)
{
    r.register_schema(10,
                      layer_schema{
                          .domain = "",
                          .optype = "ConvInteger",
                          .operands = {
                              {.name = "x", .type_param = "T1"},
                              {.name = "w", .type_param = "T2"},
                              {.name = "x_zero_point", .type_param = "T1",
                               .arity = EArity::optional},
                              {.name = "w_zero_point", .type_param = "T2",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "y", .type_param = "T3"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8}},
                              {.name = "T3", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "dilations",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "group", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });
}

static void
register_ConvTranspose(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "ConvTranspose",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "dilations",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "group", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "output_padding",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "output_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "ConvTranspose",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "dilations",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "group", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "output_padding",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "output_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "ConvTranspose",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "dilations",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "group", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "output_padding",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "output_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });
}

static void
register_Cos(schema_registry& r)
{
    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Cos",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Cos",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Cosh(schema_registry& r)
{
    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "Cosh",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Cosh",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_CumProd(schema_registry& r)
{
    r.register_schema(26,
                      layer_schema{
                          .domain = "",
                          .optype = "CumProd",
                          .operands = {
                              {.name = "x", .type_param = "T"},
                              {.name = "axis", .type_param = "T2"},
                          },
                          .results = {
                              {.name = "y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "exclusive", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "reverse", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_CumSum(schema_registry& r)
{
    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "CumSum",
                          .operands = {
                              {.name = "x", .type_param = "T"},
                              {.name = "axis", .type_param = "T2"},
                          },
                          .results = {
                              {.name = "y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "exclusive", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "reverse", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(14,
                      layer_schema{
                          .domain = "",
                          .optype = "CumSum",
                          .operands = {
                              {.name = "x", .type_param = "T"},
                              {.name = "axis", .type_param = "T2"},
                          },
                          .results = {
                              {.name = "y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "exclusive", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "reverse", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_DFT(schema_registry& r)
{
    r.register_schema(17,
                      layer_schema{
                          .domain = "",
                          .optype = "DFT",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                              {.name = "dft_length", .type_param = "T2", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "inverse", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "onesided", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(20,
                      layer_schema{
                          .domain = "",
                          .optype = "DFT",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                              {.name = "dft_length", .type_param = "T2", .arity = EArity::optional},
                              {.name = "axis", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "inverse", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "onesided", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_DeformConv(schema_registry& r)
{
    r.register_schema(19,
                      layer_schema{
                          .domain = "",
                          .optype = "DeformConv",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "offset", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                              {.name = "mask", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "dilations",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "group", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "offset_group",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "DeformConv",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "offset", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                              {.name = "mask", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "dilations",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "group", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "offset_group",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });
}

static void
register_DepthToSpace(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "DepthToSpace",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "blocksize", .type = EAttributeType::i64},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "DepthToSpace",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "blocksize", .type = EAttributeType::i64},
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"DCR"}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "DepthToSpace",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "blocksize", .type = EAttributeType::i64},
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"DCR"}}},
                          },
                      });
}

static void
register_DequantizeLinear(schema_registry& r)
{
    r.register_schema(10,
                      layer_schema{
                          .domain = "",
                          .optype = "DequantizeLinear",
                          .operands = {
                              {.name = "x", .type_param = "T"},
                              {.name = "x_scale", .type_param = "tensor(float)"},
                              {.name = "x_zero_point", .type_param = "T",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "y", .type_param = "tensor(float)"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i32}},
                              {.name = "tensor(float)", .allowed = {EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "DequantizeLinear",
                          .operands = {
                              {.name = "x", .type_param = "T"},
                              {.name = "x_scale", .type_param = "tensor(float)"},
                              {.name = "x_zero_point", .type_param = "T",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "y", .type_param = "tensor(float)"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i32}},
                              {.name = "tensor(float)", .allowed = {EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(19,
                      layer_schema{
                          .domain = "",
                          .optype = "DequantizeLinear",
                          .operands = {
                              {.name = "x", .type_param = "T1"},
                              {.name = "x_scale", .type_param = "T2"},
                              {.name = "x_zero_point", .type_param = "T1",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "y", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(21,
                      layer_schema{
                          .domain = "",
                          .optype = "DequantizeLinear",
                          .operands = {
                              {.name = "x", .type_param = "T1"},
                              {.name = "x_scale", .type_param = "T2"},
                              {.name = "x_zero_point", .type_param = "T1",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "y", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "block_size", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(23,
                      layer_schema{
                          .domain = "",
                          .optype = "DequantizeLinear",
                          .operands = {
                              {.name = "x", .type_param = "T1"},
                              {.name = "x_scale", .type_param = "T2"},
                              {.name = "x_zero_point", .type_param = "T1",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "y", .type_param = "T3"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T3", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "block_size", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "output_dtype",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(24,
                      layer_schema{
                          .domain = "",
                          .optype = "DequantizeLinear",
                          .operands = {
                              {.name = "x", .type_param = "T1"},
                              {.name = "x_scale", .type_param = "T2"},
                              {.name = "x_zero_point", .type_param = "T1",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "y", .type_param = "T3"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T3", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "block_size", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "output_dtype",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(25,
                      layer_schema{
                          .domain = "",
                          .optype = "DequantizeLinear",
                          .operands = {
                              {.name = "x", .type_param = "T1"},
                              {.name = "x_scale", .type_param = "T2"},
                              {.name = "x_zero_point", .type_param = "T1",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "y", .type_param = "T3"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T3", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "block_size", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "output_dtype",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_Det(schema_registry& r)
{
    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Det",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Det",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Div(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Div",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                              {.required = false, .name = "broadcast", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Div",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                              {.required = false, .name = "broadcast", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Div",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Div",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(14,
                      layer_schema{
                          .domain = "",
                          .optype = "Div",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Dropout(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Dropout",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                              {.name = "mask", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "is_test", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "ratio", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.5f}},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Dropout",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                              {.name = "mask", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "is_test", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "ratio", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.5f}},
                          },
                      });

    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Dropout",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                              {.name = "mask", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "ratio", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.5f}},
                          },
                      });

    r.register_schema(10,
                      layer_schema{
                          .domain = "",
                          .optype = "Dropout",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                              {.name = "mask", .type_param = "T1", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {
                              {.required = false, .name = "ratio", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.5f}},
                          },
                      });

    r.register_schema(12,
                      layer_schema{
                          .domain = "",
                          .optype = "Dropout",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "ratio", .type_param = "T1", .arity = EArity::optional},
                              {.name = "training_mode", .type_param = "T2",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                              {.name = "mask", .type_param = "T2", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::u8}},
                          },
                          .attrs = {
                              {.required = false, .name = "seed", .type = EAttributeType::i64},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Dropout",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "ratio", .type_param = "T1", .arity = EArity::optional},
                              {.name = "training_mode", .type_param = "T2",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                              {.name = "mask", .type_param = "T2", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::u8}},
                          },
                          .attrs = {
                              {.required = false, .name = "seed", .type = EAttributeType::i64},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Dropout",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "ratio", .type_param = "T1", .arity = EArity::optional},
                              {.name = "training_mode", .type_param = "T2",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                              {.name = "mask", .type_param = "T2", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::u8}},
                          },
                          .attrs = {
                              {.required = false, .name = "seed", .type = EAttributeType::i64},
                          },
                      });
}

static void
register_DynamicQuantizeLinear(schema_registry& r)
{
    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "DynamicQuantizeLinear",
                          .operands = {
                              {.name = "x", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "y", .type_param = "T2"},
                              {.name = "y_scale", .type_param = "tensor(float)"},
                              {.name = "y_zero_point", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::u8}},
                              {.name = "tensor(float)", .allowed = {EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Einsum(schema_registry& r)
{
    r.register_schema(12,
                      layer_schema{
                          .domain = "",
                          .optype = "Einsum",
                          .operands = {
                              {.name = "Inputs", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "Output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "equation", .type = EAttributeType::str},
                          },
                      });
}

static void
register_Elu(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Elu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Elu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Elu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                          },
                      });
}

static void
register_Equal(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Equal",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::u8, EDataType::i32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                              {.required = false, .name = "broadcast", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Equal",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::u8, EDataType::i32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Equal",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Equal",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });

    r.register_schema(19,
                      layer_schema{
                          .domain = "",
                          .optype = "Equal",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });
}

static void
register_Erf(schema_registry& r)
{
    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "Erf",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Erf",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Exp(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Exp",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Exp",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Exp",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Expand(schema_registry& r)
{
    r.register_schema(8,
                      layer_schema{
                          .domain = "",
                          .optype = "Expand",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "shape", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Expand",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "shape", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });
}

static void
register_EyeLike(schema_registry& r)
{
    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "EyeLike",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "dtype", .type = EAttributeType::i64},
                              {.required = false, .name = "k", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "EyeLike",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "dtype", .type = EAttributeType::i64},
                              {.required = false, .name = "k", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_Flatten(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Flatten",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "Flatten",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Flatten",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Flatten",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(21,
                      layer_schema{
                          .domain = "",
                          .optype = "Flatten",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(23,
                      layer_schema{
                          .domain = "",
                          .optype = "Flatten",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(24,
                      layer_schema{
                          .domain = "",
                          .optype = "Flatten",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(25,
                      layer_schema{
                          .domain = "",
                          .optype = "Flatten",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });
}

static void
register_Floor(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Floor",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Floor",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Floor",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_GRU(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "GRU",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "R", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                              {.name = "sequence_lens", .type_param = "T1",
                               .arity = EArity::optional},
                              {.name = "initial_h", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T", .arity = EArity::optional},
                              {.name = "Y_h", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "activation_alpha",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activation_beta",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activations",
                               .type = EAttributeType::strs},
                              {.required = false, .name = "clip", .type = EAttributeType::f32},
                              {.required = false, .name = "direction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"foward"}}},
                              {.required = false, .name = "hidden_size",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "output_sequence",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(3,
                      layer_schema{
                          .domain = "",
                          .optype = "GRU",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "R", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                              {.name = "sequence_lens", .type_param = "T1",
                               .arity = EArity::optional},
                              {.name = "initial_h", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T", .arity = EArity::optional},
                              {.name = "Y_h", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "activation_alpha",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activation_beta",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activations",
                               .type = EAttributeType::strs},
                              {.required = false, .name = "clip", .type = EAttributeType::f32},
                              {.required = false, .name = "direction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"forward"}}},
                              {.required = false, .name = "hidden_size",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "linear_before_reset",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "output_sequence",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "GRU",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "R", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                              {.name = "sequence_lens", .type_param = "T1",
                               .arity = EArity::optional},
                              {.name = "initial_h", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T", .arity = EArity::optional},
                              {.name = "Y_h", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "activation_alpha",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activation_beta",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activations",
                               .type = EAttributeType::strs},
                              {.required = false, .name = "clip", .type = EAttributeType::f32},
                              {.required = false, .name = "direction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"forward"}}},
                              {.required = false, .name = "hidden_size",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "linear_before_reset",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(14,
                      layer_schema{
                          .domain = "",
                          .optype = "GRU",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "R", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                              {.name = "sequence_lens", .type_param = "T1",
                               .arity = EArity::optional},
                              {.name = "initial_h", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T", .arity = EArity::optional},
                              {.name = "Y_h", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "activation_alpha",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activation_beta",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activations",
                               .type = EAttributeType::strs},
                              {.required = false, .name = "clip", .type = EAttributeType::f32},
                              {.required = false, .name = "direction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"forward"}}},
                              {.required = false, .name = "hidden_size",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "layout", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "linear_before_reset",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "GRU",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "R", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                              {.name = "sequence_lens", .type_param = "T1",
                               .arity = EArity::optional},
                              {.name = "initial_h", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T", .arity = EArity::optional},
                              {.name = "Y_h", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "activation_alpha",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activation_beta",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activations",
                               .type = EAttributeType::strs},
                              {.required = false, .name = "clip", .type = EAttributeType::f32},
                              {.required = false, .name = "direction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"forward"}}},
                              {.required = false, .name = "hidden_size",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "layout", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "linear_before_reset",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_Gather(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Gather",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "indices", .type_param = "Tind"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Gather",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "indices", .type_param = "Tind"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Gather",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "indices", .type_param = "Tind"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_GatherElements(schema_registry& r)
{
    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "GatherElements",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "indices", .type_param = "Tind"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "GatherElements",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "indices", .type_param = "Tind"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_GatherND(schema_registry& r)
{
    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "GatherND",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "indices", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(12,
                      layer_schema{
                          .domain = "",
                          .optype = "GatherND",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "indices", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "batch_dims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "GatherND",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "indices", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "batch_dims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_Gelu(schema_registry& r)
{
    r.register_schema(20,
                      layer_schema{
                          .domain = "",
                          .optype = "Gelu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "approximate",
                               .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"none"}}},
                          },
                      });
}

static void
register_Gemm(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Gemm",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                              {.name = "C", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "beta", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "broadcast", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "transA", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "transB", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Gemm",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                              {.name = "C", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "beta", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "broadcast", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "transA", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "transB", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Gemm",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                              {.name = "C", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "beta", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "transA", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "transB", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "Gemm",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                              {.name = "C", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "beta", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "transA", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "transB", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Gemm",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                              {.name = "C", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "beta", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "transA", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "transB", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Gemm",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                              {.name = "C", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "beta", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "transA", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "transB", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_GlobalAveragePool(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "GlobalAveragePool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "GlobalAveragePool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_GlobalLpPool(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "GlobalLpPool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "p", .type = EAttributeType::f32,
                               .default_value = attribute_value{2.0f}},
                          },
                      });

    r.register_schema(2,
                      layer_schema{
                          .domain = "",
                          .optype = "GlobalLpPool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "p", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{2}}},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "GlobalLpPool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "p", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{2}}},
                          },
                      });
}

static void
register_GlobalMaxPool(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "GlobalMaxPool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "GlobalMaxPool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Greater(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Greater",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                              {.required = false, .name = "broadcast", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Greater",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });

    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "Greater",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Greater",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });
}

static void
register_GreaterOrEqual(schema_registry& r)
{
    r.register_schema(12,
                      layer_schema{
                          .domain = "",
                          .optype = "GreaterOrEqual",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });

    r.register_schema(16,
                      layer_schema{
                          .domain = "",
                          .optype = "GreaterOrEqual",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });
}

static void
register_GridSample(schema_registry& r)
{
    r.register_schema(16,
                      layer_schema{
                          .domain = "",
                          .optype = "GridSample",
                          .operands = {
                              {.name = "X", .type_param = "T1"},
                              {.name = "grid", .type_param = "T2"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "align_corners",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"bilinear"}}},
                              {.required = false, .name = "padding_mode",
                               .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"zeros"}}},
                          },
                      });

    r.register_schema(20,
                      layer_schema{
                          .domain = "",
                          .optype = "GridSample",
                          .operands = {
                              {.name = "X", .type_param = "T1"},
                              {.name = "grid", .type_param = "T2"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "align_corners",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"linear"}}},
                              {.required = false, .name = "padding_mode",
                               .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"zeros"}}},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "GridSample",
                          .operands = {
                              {.name = "X", .type_param = "T1"},
                              {.name = "grid", .type_param = "T2"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "align_corners",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"linear"}}},
                              {.required = false, .name = "padding_mode",
                               .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"zeros"}}},
                          },
                      });
}

static void
register_GroupNormalization(schema_registry& r)
{
    r.register_schema(21,
                      layer_schema{
                          .domain = "",
                          .optype = "GroupNormalization",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "scale", .type_param = "T"},
                              {.name = "bias", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "epsilon", .type = EAttributeType::f32,
                               .default_value = attribute_value{9.999999747378752e-06f}},
                              {.required = true, .name = "num_groups", .type = EAttributeType::i64},
                              {.required = false, .name = "stash_type", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });
}

static void
register_HammingWindow(schema_registry& r)
{
    r.register_schema(17,
                      layer_schema{
                          .domain = "",
                          .optype = "HammingWindow",
                          .operands = {
                              {.name = "size", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "output_datatype",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "periodic", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });
}

static void
register_HannWindow(schema_registry& r)
{
    r.register_schema(17,
                      layer_schema{
                          .domain = "",
                          .optype = "HannWindow",
                          .operands = {
                              {.name = "size", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "output_datatype",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "periodic", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });
}

static void
register_HardSigmoid(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "HardSigmoid",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.20000000298023224f}},
                              {.required = false, .name = "beta", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.5f}},
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "HardSigmoid",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.20000000298023224f}},
                              {.required = false, .name = "beta", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.5f}},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "HardSigmoid",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.20000000298023224f}},
                              {.required = false, .name = "beta", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.5f}},
                          },
                      });
}

static void
register_HardSwish(schema_registry& r)
{
    r.register_schema(14,
                      layer_schema{
                          .domain = "",
                          .optype = "HardSwish",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "HardSwish",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Hardmax(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Hardmax",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Hardmax",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Hardmax",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{-1}}},
                          },
                      });
}

static void
register_Identity(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Identity",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Identity",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(14,
                      layer_schema{
                          .domain = "",
                          .optype = "Identity",
                          .operands = {
                              {.name = "input", .type_param = "V"},
                          },
                          .results = {
                              {.name = "output", .type_param = "V"},
                          },
                          .type_constraints = {
                              {.name = "V", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(16,
                      layer_schema{
                          .domain = "",
                          .optype = "Identity",
                          .operands = {
                              {.name = "input", .type_param = "V"},
                          },
                          .results = {
                              {.name = "output", .type_param = "V"},
                          },
                          .type_constraints = {
                              {.name = "V", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(19,
                      layer_schema{
                          .domain = "",
                          .optype = "Identity",
                          .operands = {
                              {.name = "input", .type_param = "V"},
                          },
                          .results = {
                              {.name = "output", .type_param = "V"},
                          },
                          .type_constraints = {
                              {.name = "V", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(21,
                      layer_schema{
                          .domain = "",
                          .optype = "Identity",
                          .operands = {
                              {.name = "input", .type_param = "V"},
                          },
                          .results = {
                              {.name = "output", .type_param = "V"},
                          },
                          .type_constraints = {
                              {.name = "V", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(23,
                      layer_schema{
                          .domain = "",
                          .optype = "Identity",
                          .operands = {
                              {.name = "input", .type_param = "V"},
                          },
                          .results = {
                              {.name = "output", .type_param = "V"},
                          },
                          .type_constraints = {
                              {.name = "V", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(24,
                      layer_schema{
                          .domain = "",
                          .optype = "Identity",
                          .operands = {
                              {.name = "input", .type_param = "V"},
                          },
                          .results = {
                              {.name = "output", .type_param = "V"},
                          },
                          .type_constraints = {
                              {.name = "V", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(25,
                      layer_schema{
                          .domain = "",
                          .optype = "Identity",
                          .operands = {
                              {.name = "input", .type_param = "V"},
                          },
                          .results = {
                              {.name = "output", .type_param = "V"},
                          },
                          .type_constraints = {
                              {.name = "V", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_ImageDecoder(schema_registry& r)
{
    r.register_schema(20,
                      layer_schema{
                          .domain = "",
                          .optype = "ImageDecoder",
                          .operands = {
                              {.name = "encoded_stream", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "image", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::u8}},
                              {.name = "T2", .allowed = {EDataType::u8}},
                          },
                          .attrs = {
                              {.required = false, .name = "pixel_format",
                               .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"RGB"}}},
                          },
                      });
}

static void
register_InstanceNormalization(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "InstanceNormalization",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "scale", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "epsilon", .type = EAttributeType::f32,
                               .default_value = attribute_value{9.999999747378752e-06f}},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "InstanceNormalization",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "scale", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "epsilon", .type = EAttributeType::f32,
                               .default_value = attribute_value{9.999999747378752e-06f}},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "InstanceNormalization",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "scale", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "epsilon", .type = EAttributeType::f32,
                               .default_value = attribute_value{9.999999747378752e-06f}},
                          },
                      });
}

static void
register_IsInf(schema_registry& r)
{
    r.register_schema(10,
                      layer_schema{
                          .domain = "",
                          .optype = "IsInf",
                          .operands = {
                              {.name = "X", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::u8}},
                          },
                          .attrs = {
                              {.required = false, .name = "detect_negative",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "detect_positive",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(20,
                      layer_schema{
                          .domain = "",
                          .optype = "IsInf",
                          .operands = {
                              {.name = "X", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::u8}},
                          },
                          .attrs = {
                              {.required = false, .name = "detect_negative",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "detect_positive",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });
}

static void
register_IsNaN(schema_registry& r)
{
    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "IsNaN",
                          .operands = {
                              {.name = "X", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "IsNaN",
                          .operands = {
                              {.name = "X", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });

    r.register_schema(20,
                      layer_schema{
                          .domain = "",
                          .optype = "IsNaN",
                          .operands = {
                              {.name = "X", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });
}

static void
register_LRN(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "LRN",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{9.999999747378752e-05f}},
                              {.required = false, .name = "beta", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.75f}},
                              {.required = false, .name = "bias", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = true, .name = "size", .type = EAttributeType::i64},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "LRN",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{9.999999747378752e-05f}},
                              {.required = false, .name = "beta", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.75f}},
                              {.required = false, .name = "bias", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = true, .name = "size", .type = EAttributeType::i64},
                          },
                      });
}

static void
register_LSTM(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "LSTM",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "R", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                              {.name = "sequence_lens", .type_param = "T1",
                               .arity = EArity::optional},
                              {.name = "initial_h", .type_param = "T", .arity = EArity::optional},
                              {.name = "initial_c", .type_param = "T", .arity = EArity::optional},
                              {.name = "P", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T", .arity = EArity::optional},
                              {.name = "Y_h", .type_param = "T", .arity = EArity::optional},
                              {.name = "Y_c", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "activation_alpha",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activation_beta",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activations",
                               .type = EAttributeType::strs},
                              {.required = false, .name = "clip", .type = EAttributeType::f32},
                              {.required = false, .name = "direction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"forward"}}},
                              {.required = false, .name = "hidden_size",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "input_forget",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "output_sequence",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "LSTM",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "R", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                              {.name = "sequence_lens", .type_param = "T1",
                               .arity = EArity::optional},
                              {.name = "initial_h", .type_param = "T", .arity = EArity::optional},
                              {.name = "initial_c", .type_param = "T", .arity = EArity::optional},
                              {.name = "P", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T", .arity = EArity::optional},
                              {.name = "Y_h", .type_param = "T", .arity = EArity::optional},
                              {.name = "Y_c", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "activation_alpha",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activation_beta",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activations",
                               .type = EAttributeType::strs},
                              {.required = false, .name = "clip", .type = EAttributeType::f32},
                              {.required = false, .name = "direction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"forward"}}},
                              {.required = false, .name = "hidden_size",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "input_forget",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(14,
                      layer_schema{
                          .domain = "",
                          .optype = "LSTM",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "R", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                              {.name = "sequence_lens", .type_param = "T1",
                               .arity = EArity::optional},
                              {.name = "initial_h", .type_param = "T", .arity = EArity::optional},
                              {.name = "initial_c", .type_param = "T", .arity = EArity::optional},
                              {.name = "P", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T", .arity = EArity::optional},
                              {.name = "Y_h", .type_param = "T", .arity = EArity::optional},
                              {.name = "Y_c", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "activation_alpha",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activation_beta",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activations",
                               .type = EAttributeType::strs},
                              {.required = false, .name = "clip", .type = EAttributeType::f32},
                              {.required = false, .name = "direction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"forward"}}},
                              {.required = false, .name = "hidden_size",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "input_forget",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "layout", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "LSTM",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "R", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                              {.name = "sequence_lens", .type_param = "T1",
                               .arity = EArity::optional},
                              {.name = "initial_h", .type_param = "T", .arity = EArity::optional},
                              {.name = "initial_c", .type_param = "T", .arity = EArity::optional},
                              {.name = "P", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T", .arity = EArity::optional},
                              {.name = "Y_h", .type_param = "T", .arity = EArity::optional},
                              {.name = "Y_c", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "activation_alpha",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activation_beta",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activations",
                               .type = EAttributeType::strs},
                              {.required = false, .name = "clip", .type = EAttributeType::f32},
                              {.required = false, .name = "direction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"forward"}}},
                              {.required = false, .name = "hidden_size",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "input_forget",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "layout", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_LayerNormalization(schema_registry& r)
{
    r.register_schema(17,
                      layer_schema{
                          .domain = "",
                          .optype = "LayerNormalization",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "Scale", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                              {.name = "Mean", .type_param = "U", .arity = EArity::optional},
                              {.name = "InvStdDev", .type_param = "U", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                              {.name = "U", .allowed = {EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{-1}}},
                              {.required = false, .name = "epsilon", .type = EAttributeType::f32,
                               .default_value = attribute_value{9.999999747378752e-06f}},
                              {.required = false, .name = "stash_type", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });
}

static void
register_LeakyRelu(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "LeakyRelu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.009999999776482582f}},
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "LeakyRelu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.009999999776482582f}},
                          },
                      });

    r.register_schema(16,
                      layer_schema{
                          .domain = "",
                          .optype = "LeakyRelu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.009999999776482582f}},
                          },
                      });
}

static void
register_Less(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Less",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                              {.required = false, .name = "broadcast", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Less",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });

    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "Less",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Less",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });
}

static void
register_LessOrEqual(schema_registry& r)
{
    r.register_schema(12,
                      layer_schema{
                          .domain = "",
                          .optype = "LessOrEqual",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });

    r.register_schema(16,
                      layer_schema{
                          .domain = "",
                          .optype = "LessOrEqual",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });
}

static void
register_LinearAttention(schema_registry& r)
{
    r.register_schema(27,
                      layer_schema{
                          .domain = "",
                          .optype = "LinearAttention",
                          .operands = {
                              {.name = "query", .type_param = "T"},
                              {.name = "key", .type_param = "T"},
                              {.name = "value", .type_param = "T"},
                              {.name = "past_state", .type_param = "S", .arity = EArity::optional},
                              {.name = "decay", .type_param = "T", .arity = EArity::optional},
                              {.name = "beta", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                              {.name = "present_state", .type_param = "S"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                              {.name = "S", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "chunk_size", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{64}}},
                              {.required = true, .name = "kv_num_heads",
                               .type = EAttributeType::i64},
                              {.required = true, .name = "q_num_heads",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "scale", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.0f}},
                              {.required = false, .name = "update_rule",
                               .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"gated_delta"}}},
                          },
                      });
}

static void
register_Log(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Log",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Log",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Log",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_LogSoftmax(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "LogSoftmax",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "LogSoftmax",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "LogSoftmax",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{-1}}},
                          },
                      });
}

static void
register_LpNormalization(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "LpNormalization",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{-1}}},
                              {.required = false, .name = "p", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{2}}},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "LpNormalization",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{-1}}},
                              {.required = false, .name = "p", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{2}}},
                          },
                      });
}

static void
register_LpPool(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "LpPool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "p", .type = EAttributeType::f32,
                               .default_value = attribute_value{2.0f}},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(2,
                      layer_schema{
                          .domain = "",
                          .optype = "LpPool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = true, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "p", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{2}}},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "LpPool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = true, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "p", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{2}}},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "LpPool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "ceil_mode", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "dilations",
                               .type = EAttributeType::i64s},
                              {.required = true, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "p", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{2}}},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "LpPool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "ceil_mode", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "dilations",
                               .type = EAttributeType::i64s},
                              {.required = true, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "p", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{2}}},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });
}

static void
register_MatMul(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "MatMul",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "MatMul",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "MatMul",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_MatMulInteger(schema_registry& r)
{
    r.register_schema(10,
                      layer_schema{
                          .domain = "",
                          .optype = "MatMulInteger",
                          .operands = {
                              {.name = "A", .type_param = "T1"},
                              {.name = "B", .type_param = "T2"},
                              {.name = "a_zero_point", .type_param = "T1",
                               .arity = EArity::optional},
                              {.name = "b_zero_point", .type_param = "T2",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T3"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8}},
                              {.name = "T3", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Max(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Max",
                          .operands = {
                              {.name = "data_0", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "max", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Max",
                          .operands = {
                              {.name = "data_0", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "max", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(8,
                      layer_schema{
                          .domain = "",
                          .optype = "Max",
                          .operands = {
                              {.name = "data_0", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "max", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(12,
                      layer_schema{
                          .domain = "",
                          .optype = "Max",
                          .operands = {
                              {.name = "data_0", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "max", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Max",
                          .operands = {
                              {.name = "data_0", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "max", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_MaxPool(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "MaxPool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = true, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(8,
                      layer_schema{
                          .domain = "",
                          .optype = "MaxPool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                              {.name = "Indices", .type_param = "I", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "I", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = true, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "storage_order",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(10,
                      layer_schema{
                          .domain = "",
                          .optype = "MaxPool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                              {.name = "Indices", .type_param = "I", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "I", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "ceil_mode", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "dilations",
                               .type = EAttributeType::i64s},
                              {.required = true, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "storage_order",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "MaxPool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                              {.name = "Indices", .type_param = "I", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "I", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "ceil_mode", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "dilations",
                               .type = EAttributeType::i64s},
                              {.required = true, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "storage_order",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(12,
                      layer_schema{
                          .domain = "",
                          .optype = "MaxPool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                              {.name = "Indices", .type_param = "I", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::f16, EDataType::f32}},
                              {.name = "I", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "ceil_mode", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "dilations",
                               .type = EAttributeType::i64s},
                              {.required = true, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "storage_order",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "MaxPool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                              {.name = "Indices", .type_param = "I", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                              {.name = "I", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "ceil_mode", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "dilations",
                               .type = EAttributeType::i64s},
                              {.required = true, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "storage_order",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });
}

static void
register_MaxRoiPool(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "MaxRoiPool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "rois", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "pooled_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "spatial_scale",
                               .type = EAttributeType::f32, .default_value = attribute_value{1.0f}},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "MaxRoiPool",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "rois", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "pooled_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "spatial_scale",
                               .type = EAttributeType::f32, .default_value = attribute_value{1.0f}},
                          },
                      });
}

static void
register_MaxUnpool(schema_registry& r)
{
    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "MaxUnpool",
                          .operands = {
                              {.name = "X", .type_param = "T1"},
                              {.name = "I", .type_param = "T2"},
                              {.name = "output_shape", .type_param = "T2",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = true, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "MaxUnpool",
                          .operands = {
                              {.name = "X", .type_param = "T1"},
                              {.name = "I", .type_param = "T2"},
                              {.name = "output_shape", .type_param = "T2",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = true, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "MaxUnpool",
                          .operands = {
                              {.name = "X", .type_param = "T1"},
                              {.name = "I", .type_param = "T2"},
                              {.name = "output_shape", .type_param = "T2",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = true, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });
}

static void
register_Mean(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Mean",
                          .operands = {
                              {.name = "data_0", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "mean", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Mean",
                          .operands = {
                              {.name = "data_0", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "mean", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(8,
                      layer_schema{
                          .domain = "",
                          .optype = "Mean",
                          .operands = {
                              {.name = "data_0", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "mean", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Mean",
                          .operands = {
                              {.name = "data_0", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "mean", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_MeanVarianceNormalization(schema_registry& r)
{
    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "MeanVarianceNormalization",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s,
                               .default_value = attribute_value{std::vector<std::int64_t>{std::int64_t{0}, std::int64_t{2}, std::int64_t{3}}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "MeanVarianceNormalization",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s,
                               .default_value = attribute_value{std::vector<std::int64_t>{std::int64_t{0}, std::int64_t{2}, std::int64_t{3}}}},
                          },
                      });
}

static void
register_MelWeightMatrix(schema_registry& r)
{
    r.register_schema(17,
                      layer_schema{
                          .domain = "",
                          .optype = "MelWeightMatrix",
                          .operands = {
                              {.name = "num_mel_bins", .type_param = "T1"},
                              {.name = "dft_length", .type_param = "T1"},
                              {.name = "sample_rate", .type_param = "T1"},
                              {.name = "lower_edge_hertz", .type_param = "T2"},
                              {.name = "upper_edge_hertz", .type_param = "T2"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T3"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T3", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "output_datatype",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });
}

static void
register_Min(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Min",
                          .operands = {
                              {.name = "data_0", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "min", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Min",
                          .operands = {
                              {.name = "data_0", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "min", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(8,
                      layer_schema{
                          .domain = "",
                          .optype = "Min",
                          .operands = {
                              {.name = "data_0", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "min", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(12,
                      layer_schema{
                          .domain = "",
                          .optype = "Min",
                          .operands = {
                              {.name = "data_0", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "min", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Min",
                          .operands = {
                              {.name = "data_0", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "min", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Mish(schema_registry& r)
{
    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "Mish",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Mish",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Mod(schema_registry& r)
{
    r.register_schema(10,
                      layer_schema{
                          .domain = "",
                          .optype = "Mod",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "fmod", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Mod",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "fmod", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_Mul(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Mul",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                              {.required = false, .name = "broadcast", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Mul",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                              {.required = false, .name = "broadcast", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Mul",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Mul",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(14,
                      layer_schema{
                          .domain = "",
                          .optype = "Mul",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Multinomial(schema_registry& r)
{
    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Multinomial",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "dtype", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{6}}},
                              {.required = false, .name = "sample_size",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "seed", .type = EAttributeType::f32},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Multinomial",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "dtype", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{6}}},
                              {.required = false, .name = "sample_size",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "seed", .type = EAttributeType::f32},
                          },
                      });
}

static void
register_Neg(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Neg",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Neg",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::i16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Neg",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::i16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_NegativeLogLikelihoodLoss(schema_registry& r)
{
    r.register_schema(12,
                      layer_schema{
                          .domain = "",
                          .optype = "NegativeLogLikelihoodLoss",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "target", .type_param = "Tind"},
                              {.name = "weight", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "loss", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "ignore_index",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "reduction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"mean"}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "NegativeLogLikelihoodLoss",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "target", .type_param = "Tind"},
                              {.name = "weight", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "loss", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "ignore_index",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "reduction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"mean"}}},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "NegativeLogLikelihoodLoss",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "target", .type_param = "Tind"},
                              {.name = "weight", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "loss", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "ignore_index",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "reduction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"mean"}}},
                          },
                      });
}

static void
register_NonMaxSuppression(schema_registry& r)
{
    r.register_schema(10,
                      layer_schema{
                          .domain = "",
                          .optype = "NonMaxSuppression",
                          .operands = {
                              {.name = "boxes", .type_param = "tensor(float)"},
                              {.name = "scores", .type_param = "tensor(float)"},
                              {.name = "max_output_boxes_per_class", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                              {.name = "iou_threshold", .type_param = "tensor(float)",
                               .arity = EArity::optional},
                              {.name = "score_threshold", .type_param = "tensor(float)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "selected_indices", .type_param = "tensor(int64)"},
                          },
                          .type_constraints = {
                              {.name = "tensor(float)", .allowed = {EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "center_point_box",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "NonMaxSuppression",
                          .operands = {
                              {.name = "boxes", .type_param = "tensor(float)"},
                              {.name = "scores", .type_param = "tensor(float)"},
                              {.name = "max_output_boxes_per_class", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                              {.name = "iou_threshold", .type_param = "tensor(float)",
                               .arity = EArity::optional},
                              {.name = "score_threshold", .type_param = "tensor(float)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "selected_indices", .type_param = "tensor(int64)"},
                          },
                          .type_constraints = {
                              {.name = "tensor(float)", .allowed = {EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "center_point_box",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_NonZero(schema_registry& r)
{
    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "NonZero",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "tensor(int64)"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "NonZero",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "tensor(int64)"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Not(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Not",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });
}

static void
register_OneHot(schema_registry& r)
{
    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "OneHot",
                          .operands = {
                              {.name = "indices", .type_param = "T1"},
                              {.name = "depth", .type_param = "T2"},
                              {.name = "values", .type_param = "T3"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T3"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                              {.name = "T3", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{-1}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "OneHot",
                          .operands = {
                              {.name = "indices", .type_param = "T1"},
                              {.name = "depth", .type_param = "T2"},
                              {.name = "values", .type_param = "T3"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T3"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                              {.name = "T3", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{-1}}},
                          },
                      });
}

static void
register_OptionalGetElement(schema_registry& r)
{
    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "OptionalGetElement",
                          .operands = {
                              {.name = "input", .type_param = "O"},
                          },
                          .results = {
                              {.name = "output", .type_param = "V"},
                          },
                          .type_constraints = {
                              {.name = "O", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "V", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_OptionalHasElement(schema_registry& r)
{
    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "OptionalHasElement",
                          .operands = {
                              {.name = "input", .type_param = "O", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "B"},
                          },
                          .type_constraints = {
                              {.name = "O", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "B", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });
}

static void
register_Or(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Or",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::u8}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                              {.required = false, .name = "broadcast", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Or",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::u8}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });
}

static void
register_PRelu(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "PRelu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "slope", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "PRelu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "slope", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "PRelu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "slope", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "PRelu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "slope", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(16,
                      layer_schema{
                          .domain = "",
                          .optype = "PRelu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "slope", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Pad(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Pad",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"constant"}}},
                              {.required = true, .name = "paddings", .type = EAttributeType::i64s},
                              {.required = false, .name = "value", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.0f}},
                          },
                      });

    r.register_schema(2,
                      layer_schema{
                          .domain = "",
                          .optype = "Pad",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"constant"}}},
                              {.required = true, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "value", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.0f}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Pad",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "pads", .type_param = "tensor(int64)"},
                              {.name = "constant_value", .type_param = "T",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"constant"}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Pad",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "pads", .type_param = "tensor(int64)"},
                              {.name = "constant_value", .type_param = "T",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"constant"}}},
                          },
                      });

    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "Pad",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "pads", .type_param = "tensor(int64)"},
                              {.name = "constant_value", .type_param = "T",
                               .arity = EArity::optional},
                              {.name = "axes", .type_param = "Tind", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"constant"}}},
                          },
                      });

    r.register_schema(19,
                      layer_schema{
                          .domain = "",
                          .optype = "Pad",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "pads", .type_param = "tensor(int64)"},
                              {.name = "constant_value", .type_param = "T",
                               .arity = EArity::optional},
                              {.name = "axes", .type_param = "Tind", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"constant"}}},
                          },
                      });

    r.register_schema(21,
                      layer_schema{
                          .domain = "",
                          .optype = "Pad",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "pads", .type_param = "tensor(int64)"},
                              {.name = "constant_value", .type_param = "T",
                               .arity = EArity::optional},
                              {.name = "axes", .type_param = "Tind", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"constant"}}},
                          },
                      });

    r.register_schema(23,
                      layer_schema{
                          .domain = "",
                          .optype = "Pad",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "pads", .type_param = "tensor(int64)"},
                              {.name = "constant_value", .type_param = "T",
                               .arity = EArity::optional},
                              {.name = "axes", .type_param = "Tind", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"constant"}}},
                          },
                      });

    r.register_schema(24,
                      layer_schema{
                          .domain = "",
                          .optype = "Pad",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "pads", .type_param = "tensor(int64)"},
                              {.name = "constant_value", .type_param = "T",
                               .arity = EArity::optional},
                              {.name = "axes", .type_param = "Tind", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"constant"}}},
                          },
                      });

    r.register_schema(25,
                      layer_schema{
                          .domain = "",
                          .optype = "Pad",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "pads", .type_param = "tensor(int64)"},
                              {.name = "constant_value", .type_param = "T",
                               .arity = EArity::optional},
                              {.name = "axes", .type_param = "Tind", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"constant"}}},
                          },
                      });
}

static void
register_Pow(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Pow",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "Y", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Z", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                              {.required = false, .name = "broadcast", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Pow",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "Y", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Z", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(12,
                      layer_schema{
                          .domain = "",
                          .optype = "Pow",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "Y", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "Z", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Pow",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "Y", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "Z", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(15,
                      layer_schema{
                          .domain = "",
                          .optype = "Pow",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "Y", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "Z", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_QLinearConv(schema_registry& r)
{
    r.register_schema(10,
                      layer_schema{
                          .domain = "",
                          .optype = "QLinearConv",
                          .operands = {
                              {.name = "x", .type_param = "T1"},
                              {.name = "x_scale", .type_param = "tensor(float)"},
                              {.name = "x_zero_point", .type_param = "T1"},
                              {.name = "w", .type_param = "T2"},
                              {.name = "w_scale", .type_param = "tensor(float)"},
                              {.name = "w_zero_point", .type_param = "T2"},
                              {.name = "y_scale", .type_param = "tensor(float)"},
                              {.name = "y_zero_point", .type_param = "T3"},
                              {.name = "B", .type_param = "T4", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "y", .type_param = "T3"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8}},
                              {.name = "T3", .allowed = {EDataType::i8, EDataType::u8}},
                              {.name = "T4", .allowed = {EDataType::i32}},
                              {.name = "tensor(float)", .allowed = {EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "auto_pad", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"NOTSET"}}},
                              {.required = false, .name = "dilations",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "group", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "kernel_shape",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pads", .type = EAttributeType::i64s},
                              {.required = false, .name = "strides", .type = EAttributeType::i64s},
                          },
                      });
}

static void
register_QLinearMatMul(schema_registry& r)
{
    r.register_schema(10,
                      layer_schema{
                          .domain = "",
                          .optype = "QLinearMatMul",
                          .operands = {
                              {.name = "a", .type_param = "T1"},
                              {.name = "a_scale", .type_param = "tensor(float)"},
                              {.name = "a_zero_point", .type_param = "T1"},
                              {.name = "b", .type_param = "T2"},
                              {.name = "b_scale", .type_param = "tensor(float)"},
                              {.name = "b_zero_point", .type_param = "T2"},
                              {.name = "y_scale", .type_param = "tensor(float)"},
                              {.name = "y_zero_point", .type_param = "T3"},
                          },
                          .results = {
                              {.name = "y", .type_param = "T3"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8}},
                              {.name = "T3", .allowed = {EDataType::i8, EDataType::u8}},
                              {.name = "tensor(float)", .allowed = {EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(21,
                      layer_schema{
                          .domain = "",
                          .optype = "QLinearMatMul",
                          .operands = {
                              {.name = "a", .type_param = "T1"},
                              {.name = "a_scale", .type_param = "TS"},
                              {.name = "a_zero_point", .type_param = "T1"},
                              {.name = "b", .type_param = "T2"},
                              {.name = "b_scale", .type_param = "TS"},
                              {.name = "b_zero_point", .type_param = "T2"},
                              {.name = "y_scale", .type_param = "TS"},
                              {.name = "y_zero_point", .type_param = "T3"},
                          },
                          .results = {
                              {.name = "y", .type_param = "T3"},
                          },
                          .type_constraints = {
                              {.name = "TS", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8}},
                              {.name = "T3", .allowed = {EDataType::i8, EDataType::u8}},
                          },
                          .attrs = {},
                      });
}

static void
register_QuantizeLinear(schema_registry& r)
{
    r.register_schema(10,
                      layer_schema{
                          .domain = "",
                          .optype = "QuantizeLinear",
                          .operands = {
                              {.name = "x", .type_param = "T1"},
                              {.name = "y_scale", .type_param = "tensor(float)"},
                              {.name = "y_zero_point", .type_param = "T2",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "y", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i32, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8}},
                              {.name = "tensor(float)", .allowed = {EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "QuantizeLinear",
                          .operands = {
                              {.name = "x", .type_param = "T1"},
                              {.name = "y_scale", .type_param = "tensor(float)"},
                              {.name = "y_zero_point", .type_param = "T2",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "y", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i32, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8}},
                              {.name = "tensor(float)", .allowed = {EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(19,
                      layer_schema{
                          .domain = "",
                          .optype = "QuantizeLinear",
                          .operands = {
                              {.name = "x", .type_param = "T1"},
                              {.name = "y_scale", .type_param = "T1"},
                              {.name = "y_zero_point", .type_param = "T2",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "y", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "saturate", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(21,
                      layer_schema{
                          .domain = "",
                          .optype = "QuantizeLinear",
                          .operands = {
                              {.name = "x", .type_param = "T1"},
                              {.name = "y_scale", .type_param = "T1"},
                              {.name = "y_zero_point", .type_param = "T2",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "y", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "block_size", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "output_dtype",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "saturate", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(23,
                      layer_schema{
                          .domain = "",
                          .optype = "QuantizeLinear",
                          .operands = {
                              {.name = "x", .type_param = "T1"},
                              {.name = "y_scale", .type_param = "T2"},
                              {.name = "y_zero_point", .type_param = "T3",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "y", .type_param = "T3"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T3", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "block_size", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "output_dtype",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "precision", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "saturate", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(24,
                      layer_schema{
                          .domain = "",
                          .optype = "QuantizeLinear",
                          .operands = {
                              {.name = "x", .type_param = "T1"},
                              {.name = "y_scale", .type_param = "T2"},
                              {.name = "y_zero_point", .type_param = "T3",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "y", .type_param = "T3"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T3", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "block_size", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "output_dtype",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "precision", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "saturate", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(25,
                      layer_schema{
                          .domain = "",
                          .optype = "QuantizeLinear",
                          .operands = {
                              {.name = "x", .type_param = "T1"},
                              {.name = "y_scale", .type_param = "T2"},
                              {.name = "y_zero_point", .type_param = "T3",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "y", .type_param = "T3"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T3", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "block_size", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "output_dtype",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "precision", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "saturate", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });
}

static void
register_RMSNormalization(schema_registry& r)
{
    r.register_schema(23,
                      layer_schema{
                          .domain = "",
                          .optype = "RMSNormalization",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "scale", .type_param = "V"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "V"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                              {.name = "V", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{-1}}},
                              {.required = false, .name = "epsilon", .type = EAttributeType::f32,
                               .default_value = attribute_value{9.999999747378752e-06f}},
                              {.required = false, .name = "stash_type", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });
}

static void
register_RNN(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "RNN",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "R", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                              {.name = "sequence_lens", .type_param = "T1",
                               .arity = EArity::optional},
                              {.name = "initial_h", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T", .arity = EArity::optional},
                              {.name = "Y_h", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "activation_alpha",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activation_beta",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activations",
                               .type = EAttributeType::strs,
                               .default_value = attribute_value{std::vector<std::string>{std::string{"Tanh"}, std::string{"Tanh"}}}},
                              {.required = false, .name = "clip", .type = EAttributeType::f32},
                              {.required = false, .name = "direction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"forward"}}},
                              {.required = false, .name = "hidden_size",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "output_sequence",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "RNN",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "R", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                              {.name = "sequence_lens", .type_param = "T1",
                               .arity = EArity::optional},
                              {.name = "initial_h", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T", .arity = EArity::optional},
                              {.name = "Y_h", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "activation_alpha",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activation_beta",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activations",
                               .type = EAttributeType::strs,
                               .default_value = attribute_value{std::vector<std::string>{std::string{"Tanh"}, std::string{"Tanh"}}}},
                              {.required = false, .name = "clip", .type = EAttributeType::f32},
                              {.required = false, .name = "direction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"forward"}}},
                              {.required = false, .name = "hidden_size",
                               .type = EAttributeType::i64},
                          },
                      });

    r.register_schema(14,
                      layer_schema{
                          .domain = "",
                          .optype = "RNN",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "R", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                              {.name = "sequence_lens", .type_param = "T1",
                               .arity = EArity::optional},
                              {.name = "initial_h", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T", .arity = EArity::optional},
                              {.name = "Y_h", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "activation_alpha",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activation_beta",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activations",
                               .type = EAttributeType::strs,
                               .default_value = attribute_value{std::vector<std::string>{std::string{"Tanh"}, std::string{"Tanh"}}}},
                              {.required = false, .name = "clip", .type = EAttributeType::f32},
                              {.required = false, .name = "direction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"forward"}}},
                              {.required = false, .name = "hidden_size",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "layout", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "RNN",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "W", .type_param = "T"},
                              {.name = "R", .type_param = "T"},
                              {.name = "B", .type_param = "T", .arity = EArity::optional},
                              {.name = "sequence_lens", .type_param = "T1",
                               .arity = EArity::optional},
                              {.name = "initial_h", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T", .arity = EArity::optional},
                              {.name = "Y_h", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "activation_alpha",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activation_beta",
                               .type = EAttributeType::f32s},
                              {.required = false, .name = "activations",
                               .type = EAttributeType::strs,
                               .default_value = attribute_value{std::vector<std::string>{std::string{"Tanh"}, std::string{"Tanh"}}}},
                              {.required = false, .name = "clip", .type = EAttributeType::f32},
                              {.required = false, .name = "direction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"forward"}}},
                              {.required = false, .name = "hidden_size",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "layout", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_RandomNormal(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "RandomNormal",
                          .operands = {},
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "dtype", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "mean", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.0f}},
                              {.required = false, .name = "scale", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "seed", .type = EAttributeType::f32},
                              {.required = true, .name = "shape", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "RandomNormal",
                          .operands = {},
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "dtype", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "mean", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.0f}},
                              {.required = false, .name = "scale", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "seed", .type = EAttributeType::f32},
                              {.required = true, .name = "shape", .type = EAttributeType::i64s},
                          },
                      });
}

static void
register_RandomNormalLike(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "RandomNormalLike",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "dtype", .type = EAttributeType::i64},
                              {.required = false, .name = "mean", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.0f}},
                              {.required = false, .name = "scale", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "seed", .type = EAttributeType::f32},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "RandomNormalLike",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "dtype", .type = EAttributeType::i64},
                              {.required = false, .name = "mean", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.0f}},
                              {.required = false, .name = "scale", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "seed", .type = EAttributeType::f32},
                          },
                      });
}

static void
register_RandomUniform(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "RandomUniform",
                          .operands = {},
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "dtype", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "high", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "low", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.0f}},
                              {.required = false, .name = "seed", .type = EAttributeType::f32},
                              {.required = true, .name = "shape", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "RandomUniform",
                          .operands = {},
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "dtype", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "high", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "low", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.0f}},
                              {.required = false, .name = "seed", .type = EAttributeType::f32},
                              {.required = true, .name = "shape", .type = EAttributeType::i64s},
                          },
                      });
}

static void
register_RandomUniformLike(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "RandomUniformLike",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "dtype", .type = EAttributeType::i64},
                              {.required = false, .name = "high", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "low", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.0f}},
                              {.required = false, .name = "seed", .type = EAttributeType::f32},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "RandomUniformLike",
                          .operands = {
                              {.name = "input", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T2"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "dtype", .type = EAttributeType::i64},
                              {.required = false, .name = "high", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                              {.required = false, .name = "low", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.0f}},
                              {.required = false, .name = "seed", .type = EAttributeType::f32},
                          },
                      });
}

static void
register_Range(schema_registry& r)
{
    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Range",
                          .operands = {
                              {.name = "start", .type_param = "T"},
                              {.name = "limit", .type_param = "T"},
                              {.name = "delta", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i16, EDataType::i32,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(27,
                      layer_schema{
                          .domain = "",
                          .optype = "Range",
                          .operands = {
                              {.name = "start", .type_param = "T"},
                              {.name = "limit", .type_param = "T"},
                              {.name = "delta", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i16, EDataType::i32,
                                                        EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "stash_type", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });
}

static void
register_Reciprocal(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Reciprocal",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Reciprocal",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Reciprocal",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_ReduceL1(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceL1",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceL1",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceL1",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceL1",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "noop_with_empty_axes",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_ReduceL2(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceL2",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceL2",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceL2",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceL2",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "noop_with_empty_axes",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_ReduceLogSum(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceLogSum",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceLogSum",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceLogSum",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceLogSum",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "noop_with_empty_axes",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_ReduceLogSumExp(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceLogSumExp",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceLogSumExp",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceLogSumExp",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceLogSumExp",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "noop_with_empty_axes",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_ReduceMax(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceMax",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceMax",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(12,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceMax",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceMax",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceMax",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "noop_with_empty_axes",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(20,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceMax",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "noop_with_empty_axes",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_ReduceMean(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceMean",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceMean",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceMean",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceMean",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "noop_with_empty_axes",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_ReduceMin(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceMin",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceMin",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(12,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceMin",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceMin",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceMin",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "noop_with_empty_axes",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(20,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceMin",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "noop_with_empty_axes",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_ReduceProd(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceProd",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceProd",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceProd",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceProd",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "noop_with_empty_axes",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_ReduceSum(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceSum",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceSum",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceSum",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "noop_with_empty_axes",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_ReduceSumSquare(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceSumSquare",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceSumSquare",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceSumSquare",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "ReduceSumSquare",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "reduced", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "keepdims", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "noop_with_empty_axes",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_Relu(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Relu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Relu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Relu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(14,
                      layer_schema{
                          .domain = "",
                          .optype = "Relu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::i16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Reshape(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Reshape",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "reshaped", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "shape", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(5,
                      layer_schema{
                          .domain = "",
                          .optype = "Reshape",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "shape", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "reshaped", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Reshape",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "shape", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "reshaped", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(14,
                      layer_schema{
                          .domain = "",
                          .optype = "Reshape",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "shape", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "reshaped", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "allowzero", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(19,
                      layer_schema{
                          .domain = "",
                          .optype = "Reshape",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "shape", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "reshaped", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "allowzero", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(21,
                      layer_schema{
                          .domain = "",
                          .optype = "Reshape",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "shape", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "reshaped", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "allowzero", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(23,
                      layer_schema{
                          .domain = "",
                          .optype = "Reshape",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "shape", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "reshaped", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "allowzero", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(24,
                      layer_schema{
                          .domain = "",
                          .optype = "Reshape",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "shape", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "reshaped", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "allowzero", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(25,
                      layer_schema{
                          .domain = "",
                          .optype = "Reshape",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "shape", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "reshaped", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "allowzero", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_Resize(schema_registry& r)
{
    r.register_schema(10,
                      layer_schema{
                          .domain = "",
                          .optype = "Resize",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "scales", .type_param = "tensor(float)"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "tensor(float)", .allowed = {EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"nearest"}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Resize",
                          .operands = {
                              {.name = "X", .type_param = "T1"},
                              {.name = "roi", .type_param = "T2"},
                              {.name = "scales", .type_param = "tensor(float)"},
                              {.name = "sizes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "tensor(float)", .allowed = {EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "coordinate_transformation_mode",
                               .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"half_pixel"}}},
                              {.required = false, .name = "cubic_coeff_a",
                               .type = EAttributeType::f32,
                               .default_value = attribute_value{-0.75f}},
                              {.required = false, .name = "exclude_outside",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "extrapolation_value",
                               .type = EAttributeType::f32, .default_value = attribute_value{0.0f}},
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"nearest"}}},
                              {.required = false, .name = "nearest_mode",
                               .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"round_prefer_floor"}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Resize",
                          .operands = {
                              {.name = "X", .type_param = "T1"},
                              {.name = "roi", .type_param = "T2", .arity = EArity::optional},
                              {.name = "scales", .type_param = "tensor(float)",
                               .arity = EArity::optional},
                              {.name = "sizes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "tensor(float)", .allowed = {EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "coordinate_transformation_mode",
                               .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"half_pixel"}}},
                              {.required = false, .name = "cubic_coeff_a",
                               .type = EAttributeType::f32,
                               .default_value = attribute_value{-0.75f}},
                              {.required = false, .name = "exclude_outside",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "extrapolation_value",
                               .type = EAttributeType::f32, .default_value = attribute_value{0.0f}},
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"nearest"}}},
                              {.required = false, .name = "nearest_mode",
                               .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"round_prefer_floor"}}},
                          },
                      });

    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "Resize",
                          .operands = {
                              {.name = "X", .type_param = "T1"},
                              {.name = "roi", .type_param = "T2", .arity = EArity::optional},
                              {.name = "scales", .type_param = "tensor(float)",
                               .arity = EArity::optional},
                              {.name = "sizes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "tensor(float)", .allowed = {EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "antialias", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "coordinate_transformation_mode",
                               .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"half_pixel"}}},
                              {.required = false, .name = "cubic_coeff_a",
                               .type = EAttributeType::f32,
                               .default_value = attribute_value{-0.75f}},
                              {.required = false, .name = "exclude_outside",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "extrapolation_value",
                               .type = EAttributeType::f32, .default_value = attribute_value{0.0f}},
                              {.required = false, .name = "keep_aspect_ratio_policy",
                               .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"stretch"}}},
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"nearest"}}},
                              {.required = false, .name = "nearest_mode",
                               .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"round_prefer_floor"}}},
                          },
                      });

    r.register_schema(19,
                      layer_schema{
                          .domain = "",
                          .optype = "Resize",
                          .operands = {
                              {.name = "X", .type_param = "T1"},
                              {.name = "roi", .type_param = "T2", .arity = EArity::optional},
                              {.name = "scales", .type_param = "tensor(float)",
                               .arity = EArity::optional},
                              {.name = "sizes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::i8, EDataType::u8,
                                                         EDataType::i16, EDataType::u16,
                                                         EDataType::i32, EDataType::f16,
                                                         EDataType::bf16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "tensor(float)", .allowed = {EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "antialias", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = false, .name = "coordinate_transformation_mode",
                               .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"half_pixel"}}},
                              {.required = false, .name = "cubic_coeff_a",
                               .type = EAttributeType::f32,
                               .default_value = attribute_value{-0.75f}},
                              {.required = false, .name = "exclude_outside",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "extrapolation_value",
                               .type = EAttributeType::f32, .default_value = attribute_value{0.0f}},
                              {.required = false, .name = "keep_aspect_ratio_policy",
                               .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"stretch"}}},
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"nearest"}}},
                              {.required = false, .name = "nearest_mode",
                               .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"round_prefer_floor"}}},
                          },
                      });
}

static void
register_ReverseSequence(schema_registry& r)
{
    r.register_schema(10,
                      layer_schema{
                          .domain = "",
                          .optype = "ReverseSequence",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "sequence_lens", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "batch_axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "time_axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_RoiAlign(schema_registry& r)
{
    r.register_schema(10,
                      layer_schema{
                          .domain = "",
                          .optype = "RoiAlign",
                          .operands = {
                              {.name = "X", .type_param = "T1"},
                              {.name = "rois", .type_param = "T1"},
                              {.name = "batch_indices", .type_param = "T2"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"avg"}}},
                              {.required = false, .name = "output_height",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "output_width",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "sampling_ratio",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "spatial_scale",
                               .type = EAttributeType::f32, .default_value = attribute_value{1.0f}},
                          },
                      });

    r.register_schema(16,
                      layer_schema{
                          .domain = "",
                          .optype = "RoiAlign",
                          .operands = {
                              {.name = "X", .type_param = "T1"},
                              {.name = "rois", .type_param = "T1"},
                              {.name = "batch_indices", .type_param = "T2"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "coordinate_transformation_mode",
                               .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"half_pixel"}}},
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"avg"}}},
                              {.required = false, .name = "output_height",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "output_width",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "sampling_ratio",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "spatial_scale",
                               .type = EAttributeType::f32, .default_value = attribute_value{1.0f}},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "RoiAlign",
                          .operands = {
                              {.name = "X", .type_param = "T1"},
                              {.name = "rois", .type_param = "T1"},
                              {.name = "batch_indices", .type_param = "T2"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "coordinate_transformation_mode",
                               .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"half_pixel"}}},
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"avg"}}},
                              {.required = false, .name = "output_height",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "output_width",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "sampling_ratio",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "spatial_scale",
                               .type = EAttributeType::f32, .default_value = attribute_value{1.0f}},
                          },
                      });
}

static void
register_RotaryEmbedding(schema_registry& r)
{
    r.register_schema(23,
                      layer_schema{
                          .domain = "",
                          .optype = "RotaryEmbedding",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "cos_cache", .type_param = "T"},
                              {.name = "sin_cache", .type_param = "T"},
                              {.name = "position_ids", .type_param = "M",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                              {.name = "M", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "interleaved",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "num_heads", .type = EAttributeType::i64},
                              {.required = false, .name = "rotary_embedding_dim",
                               .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_Round(schema_registry& r)
{
    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Round",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Round",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_STFT(schema_registry& r)
{
    r.register_schema(17,
                      layer_schema{
                          .domain = "",
                          .optype = "STFT",
                          .operands = {
                              {.name = "signal", .type_param = "T1"},
                              {.name = "frame_step", .type_param = "T2"},
                              {.name = "window", .type_param = "T1", .arity = EArity::optional},
                              {.name = "frame_length", .type_param = "T2",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T1", .allowed = {EDataType::f16, EDataType::bf16,
                                                         EDataType::f32}},
                              {.name = "T2", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "onesided", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });
}

static void
register_Scatter(schema_registry& r)
{
    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "Scatter",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "indices", .type_param = "Tind"},
                              {.name = "updates", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_ScatterElements(schema_registry& r)
{
    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "ScatterElements",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "indices", .type_param = "Tind"},
                              {.name = "updates", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "ScatterElements",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "indices", .type_param = "Tind"},
                              {.name = "updates", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(16,
                      layer_schema{
                          .domain = "",
                          .optype = "ScatterElements",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "indices", .type_param = "Tind"},
                              {.name = "updates", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "reduction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"none"}}},
                          },
                      });

    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "ScatterElements",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "indices", .type_param = "Tind"},
                              {.name = "updates", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "reduction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"none"}}},
                          },
                      });
}

static void
register_ScatterND(schema_registry& r)
{
    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "ScatterND",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "indices", .type_param = "tensor(int64)"},
                              {.name = "updates", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "ScatterND",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "indices", .type_param = "tensor(int64)"},
                              {.name = "updates", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(16,
                      layer_schema{
                          .domain = "",
                          .optype = "ScatterND",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "indices", .type_param = "tensor(int64)"},
                              {.name = "updates", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "reduction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"none"}}},
                          },
                      });

    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "ScatterND",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "indices", .type_param = "tensor(int64)"},
                              {.name = "updates", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "reduction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"none"}}},
                          },
                      });
}

static void
register_Selu(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Selu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.673200011253357f}},
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "gamma", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0506999492645264f}},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Selu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.6732631921768188f}},
                              {.required = false, .name = "gamma", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0507010221481323f}},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Selu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.6732631921768188f}},
                              {.required = false, .name = "gamma", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0507010221481323f}},
                          },
                      });
}

static void
register_Shape(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Shape",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "shape", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Shape",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "shape", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(15,
                      layer_schema{
                          .domain = "",
                          .optype = "Shape",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "shape", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "end", .type = EAttributeType::i64},
                              {.required = false, .name = "start", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(19,
                      layer_schema{
                          .domain = "",
                          .optype = "Shape",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "shape", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "end", .type = EAttributeType::i64},
                              {.required = false, .name = "start", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(21,
                      layer_schema{
                          .domain = "",
                          .optype = "Shape",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "shape", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "end", .type = EAttributeType::i64},
                              {.required = false, .name = "start", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(23,
                      layer_schema{
                          .domain = "",
                          .optype = "Shape",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "shape", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "end", .type = EAttributeType::i64},
                              {.required = false, .name = "start", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(24,
                      layer_schema{
                          .domain = "",
                          .optype = "Shape",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "shape", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "end", .type = EAttributeType::i64},
                              {.required = false, .name = "start", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(25,
                      layer_schema{
                          .domain = "",
                          .optype = "Shape",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "shape", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "end", .type = EAttributeType::i64},
                              {.required = false, .name = "start", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });
}

static void
register_Shrink(schema_registry& r)
{
    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "Shrink",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "bias", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.0f}},
                              {.required = false, .name = "lambd", .type = EAttributeType::f32,
                               .default_value = attribute_value{0.5f}},
                          },
                      });
}

static void
register_Sigmoid(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Sigmoid",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Sigmoid",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Sigmoid",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Sign(schema_registry& r)
{
    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "Sign",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Sign",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Sin(schema_registry& r)
{
    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Sin",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Sin",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Sinh(schema_registry& r)
{
    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "Sinh",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Sinh",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Size(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Size",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "size", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Size",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "size", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(19,
                      layer_schema{
                          .domain = "",
                          .optype = "Size",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "size", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(21,
                      layer_schema{
                          .domain = "",
                          .optype = "Size",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "size", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(23,
                      layer_schema{
                          .domain = "",
                          .optype = "Size",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "size", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(24,
                      layer_schema{
                          .domain = "",
                          .optype = "Size",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "size", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(25,
                      layer_schema{
                          .domain = "",
                          .optype = "Size",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "size", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Slice(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Slice",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                              {.required = true, .name = "ends", .type = EAttributeType::i64s},
                              {.required = true, .name = "starts", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(10,
                      layer_schema{
                          .domain = "",
                          .optype = "Slice",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "starts", .type_param = "Tind"},
                              {.name = "ends", .type_param = "Tind"},
                              {.name = "axes", .type_param = "Tind", .arity = EArity::optional},
                              {.name = "steps", .type_param = "Tind", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Slice",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "starts", .type_param = "Tind"},
                              {.name = "ends", .type_param = "Tind"},
                              {.name = "axes", .type_param = "Tind", .arity = EArity::optional},
                              {.name = "steps", .type_param = "Tind", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Slice",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "starts", .type_param = "Tind"},
                              {.name = "ends", .type_param = "Tind"},
                              {.name = "axes", .type_param = "Tind", .arity = EArity::optional},
                              {.name = "steps", .type_param = "Tind", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Softmax(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Softmax",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Softmax",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Softmax",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{-1}}},
                          },
                      });
}

static void
register_SoftmaxCrossEntropyLoss(schema_registry& r)
{
    r.register_schema(12,
                      layer_schema{
                          .domain = "",
                          .optype = "SoftmaxCrossEntropyLoss",
                          .operands = {
                              {.name = "scores", .type_param = "T"},
                              {.name = "labels", .type_param = "Tind"},
                              {.name = "weights", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                              {.name = "log_prob", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "ignore_index",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "reduction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"mean"}}},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "SoftmaxCrossEntropyLoss",
                          .operands = {
                              {.name = "scores", .type_param = "T"},
                              {.name = "labels", .type_param = "Tind"},
                              {.name = "weights", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                              {.name = "log_prob", .type_param = "T", .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                              {.name = "Tind", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "ignore_index",
                               .type = EAttributeType::i64},
                              {.required = false, .name = "reduction", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"mean"}}},
                          },
                      });
}

static void
register_Softplus(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Softplus",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Softplus",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Softsign(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Softsign",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Softsign",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_SpaceToDepth(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "SpaceToDepth",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "blocksize", .type = EAttributeType::i64},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "SpaceToDepth",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "blocksize", .type = EAttributeType::i64},
                          },
                      });
}

static void
register_Split(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Split",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "split", .type_param = "T", .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "outputs...", .type_param = "T", .arity = EArity::variadic},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                              {.required = false, .name = "split", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(2,
                      layer_schema{
                          .domain = "",
                          .optype = "Split",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "outputs", .type_param = "T", .arity = EArity::variadic},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "split", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Split",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "outputs", .type_param = "T", .arity = EArity::variadic},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "split", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Split",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "split", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "outputs", .type_param = "T", .arity = EArity::variadic},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(18,
                      layer_schema{
                          .domain = "",
                          .optype = "Split",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "split", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "outputs", .type_param = "T", .arity = EArity::variadic},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "num_outputs",
                               .type = EAttributeType::i64},
                          },
                      });
}

static void
register_Sqrt(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Sqrt",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Sqrt",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Sqrt",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Squeeze(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Squeeze",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "squeezed", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Squeeze",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "squeezed", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axes", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Squeeze",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "squeezed", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(21,
                      layer_schema{
                          .domain = "",
                          .optype = "Squeeze",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "squeezed", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(23,
                      layer_schema{
                          .domain = "",
                          .optype = "Squeeze",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "squeezed", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(24,
                      layer_schema{
                          .domain = "",
                          .optype = "Squeeze",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "squeezed", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(25,
                      layer_schema{
                          .domain = "",
                          .optype = "Squeeze",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "squeezed", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Sub(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Sub",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                              {.required = false, .name = "broadcast", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Sub",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                              {.required = false, .name = "broadcast", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Sub",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Sub",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(14,
                      layer_schema{
                          .domain = "",
                          .optype = "Sub",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Sum(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Sum",
                          .operands = {
                              {.name = "data_0", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "sum", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Sum",
                          .operands = {
                              {.name = "data_0", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "sum", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(8,
                      layer_schema{
                          .domain = "",
                          .optype = "Sum",
                          .operands = {
                              {.name = "data_0", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "sum", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Sum",
                          .operands = {
                              {.name = "data_0", .type_param = "T", .arity = EArity::variadic},
                          },
                          .results = {
                              {.name = "sum", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Swish(schema_registry& r)
{
    r.register_schema(24,
                      layer_schema{
                          .domain = "",
                          .optype = "Swish",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                          },
                      });
}

static void
register_Tan(schema_registry& r)
{
    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Tan",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "Tan",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Tanh(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Tanh",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "consumed_inputs",
                               .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Tanh",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Tanh",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_TensorScatter(schema_registry& r)
{
    r.register_schema(24,
                      layer_schema{
                          .domain = "",
                          .optype = "TensorScatter",
                          .operands = {
                              {.name = "past_cache", .type_param = "T"},
                              {.name = "update", .type_param = "T"},
                              {.name = "write_indices", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "present_cache", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{-2}}},
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"linear"}}},
                          },
                      });
}

static void
register_TfIdfVectorizer(schema_registry& r)
{
    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "TfIdfVectorizer",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i32}},
                              {.name = "T1", .allowed = {EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "max_gram_length",
                               .type = EAttributeType::i64},
                              {.required = true, .name = "max_skip_count",
                               .type = EAttributeType::i64},
                              {.required = true, .name = "min_gram_length",
                               .type = EAttributeType::i64},
                              {.required = true, .name = "mode", .type = EAttributeType::str},
                              {.required = true, .name = "ngram_counts",
                               .type = EAttributeType::i64s},
                              {.required = true, .name = "ngram_indexes",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pool_int64s",
                               .type = EAttributeType::i64s},
                              {.required = false, .name = "pool_strings",
                               .type = EAttributeType::strs},
                              {.required = false, .name = "weights", .type = EAttributeType::f32s},
                          },
                      });
}

static void
register_ThresholdedRelu(schema_registry& r)
{
    r.register_schema(10,
                      layer_schema{
                          .domain = "",
                          .optype = "ThresholdedRelu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                          },
                      });

    r.register_schema(22,
                      layer_schema{
                          .domain = "",
                          .optype = "ThresholdedRelu",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::bf16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "alpha", .type = EAttributeType::f32,
                               .default_value = attribute_value{1.0f}},
                          },
                      });
}

static void
register_Tile(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Tile",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "tiles", .type_param = "T"},
                              {.name = "axis", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(6,
                      layer_schema{
                          .domain = "",
                          .optype = "Tile",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "repeats", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Tile",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "repeats", .type_param = "T1"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "T1", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });
}

static void
register_TopK(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "TopK",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Values", .type_param = "T"},
                              {.name = "Indices", .type_param = "I"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "I", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{-1}}},
                              {.required = true, .name = "k", .type = EAttributeType::i64},
                          },
                      });

    r.register_schema(10,
                      layer_schema{
                          .domain = "",
                          .optype = "TopK",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "K", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "Values", .type_param = "T"},
                              {.name = "Indices", .type_param = "I"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::f16, EDataType::f32}},
                              {.name = "I", .allowed = {EDataType::i32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{-1}}},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "TopK",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "K", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "Values", .type_param = "T"},
                              {.name = "Indices", .type_param = "I"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "I", .allowed = {EDataType::i32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{-1}}},
                              {.required = false, .name = "largest", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "sorted", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });

    r.register_schema(24,
                      layer_schema{
                          .domain = "",
                          .optype = "TopK",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "K", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "Values", .type_param = "T"},
                              {.name = "Indices", .type_param = "I"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "I", .allowed = {EDataType::i32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{-1}}},
                              {.required = false, .name = "largest", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                              {.required = false, .name = "sorted", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });
}

static void
register_Transpose(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Transpose",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "transposed", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "perm", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Transpose",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "transposed", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "perm", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(21,
                      layer_schema{
                          .domain = "",
                          .optype = "Transpose",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "transposed", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "perm", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(23,
                      layer_schema{
                          .domain = "",
                          .optype = "Transpose",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "transposed", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "perm", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(24,
                      layer_schema{
                          .domain = "",
                          .optype = "Transpose",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "transposed", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "perm", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(25,
                      layer_schema{
                          .domain = "",
                          .optype = "Transpose",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "transposed", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "perm", .type = EAttributeType::i64s},
                          },
                      });
}

static void
register_Trilu(schema_registry& r)
{
    r.register_schema(14,
                      layer_schema{
                          .domain = "",
                          .optype = "Trilu",
                          .operands = {
                              {.name = "input", .type_param = "T"},
                              {.name = "k", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "upper", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });
}

static void
register_Unique(schema_registry& r)
{
    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Unique",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                              {.name = "indices", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                              {.name = "inverse_indices", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                              {.name = "counts", .type_param = "tensor(int64)",
                               .arity = EArity::optional},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                              {.required = false, .name = "sorted", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{1}}},
                          },
                      });
}

static void
register_Unsqueeze(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Unsqueeze",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "expanded", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "axes", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(11,
                      layer_schema{
                          .domain = "",
                          .optype = "Unsqueeze",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                          },
                          .results = {
                              {.name = "expanded", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = true, .name = "axes", .type = EAttributeType::i64s},
                          },
                      });

    r.register_schema(13,
                      layer_schema{
                          .domain = "",
                          .optype = "Unsqueeze",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "expanded", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(21,
                      layer_schema{
                          .domain = "",
                          .optype = "Unsqueeze",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "expanded", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(23,
                      layer_schema{
                          .domain = "",
                          .optype = "Unsqueeze",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "expanded", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(24,
                      layer_schema{
                          .domain = "",
                          .optype = "Unsqueeze",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "expanded", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(25,
                      layer_schema{
                          .domain = "",
                          .optype = "Unsqueeze",
                          .operands = {
                              {.name = "data", .type_param = "T"},
                              {.name = "axes", .type_param = "tensor(int64)"},
                          },
                          .results = {
                              {.name = "expanded", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                              {.name = "tensor(int64)", .allowed = {EDataType::i32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Upsample(schema_registry& r)
{
    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Upsample",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"nearest"}}},
                              {.required = true, .name = "scales", .type = EAttributeType::f32s},
                          },
                      });

    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "Upsample",
                          .operands = {
                              {.name = "X", .type_param = "T"},
                              {.name = "scales", .type_param = "tensor(float)"},
                          },
                          .results = {
                              {.name = "Y", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                              {.name = "tensor(float)", .allowed = {EDataType::f32}},
                          },
                          .attrs = {
                              {.required = false, .name = "mode", .type = EAttributeType::str,
                               .default_value = attribute_value{std::string{"nearest"}}},
                          },
                      });
}

static void
register_Where(schema_registry& r)
{
    r.register_schema(9,
                      layer_schema{
                          .domain = "",
                          .optype = "Where",
                          .operands = {
                              {.name = "condition", .type_param = "B"},
                              {.name = "X", .type_param = "T"},
                              {.name = "Y", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "B", .allowed = {EDataType::u8}},
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::f32}},
                          },
                          .attrs = {},
                      });

    r.register_schema(16,
                      layer_schema{
                          .domain = "",
                          .optype = "Where",
                          .operands = {
                              {.name = "condition", .type_param = "B"},
                              {.name = "X", .type_param = "T"},
                              {.name = "Y", .type_param = "T"},
                          },
                          .results = {
                              {.name = "output", .type_param = "T"},
                          },
                          .type_constraints = {
                              {.name = "B", .allowed = {EDataType::u8}},
                              {.name = "T", .allowed = {EDataType::i8, EDataType::u8,
                                                        EDataType::i16, EDataType::u16,
                                                        EDataType::i32, EDataType::f16,
                                                        EDataType::bf16, EDataType::f32}},
                          },
                          .attrs = {},
                      });
}

static void
register_Xor(schema_registry& r)
{
    r.register_schema(1,
                      layer_schema{
                          .domain = "",
                          .optype = "Xor",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::u8}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {
                              {.required = false, .name = "axis", .type = EAttributeType::i64},
                              {.required = false, .name = "broadcast", .type = EAttributeType::i64,
                               .default_value = attribute_value{std::int64_t{0}}},
                          },
                      });

    r.register_schema(7,
                      layer_schema{
                          .domain = "",
                          .optype = "Xor",
                          .operands = {
                              {.name = "A", .type_param = "T"},
                              {.name = "B", .type_param = "T"},
                          },
                          .results = {
                              {.name = "C", .type_param = "T1"},
                          },
                          .type_constraints = {
                              {.name = "T", .allowed = {EDataType::u8}},
                              {.name = "T1", .allowed = {EDataType::u8}},
                          },
                          .attrs = {},
                      });
}

void
register_onnx_schemas(schema_registry& registry)
{
    register_Abs(registry);
    register_Acos(registry);
    register_Acosh(registry);
    register_Add(registry);
    register_AffineGrid(registry);
    register_And(registry);
    register_ArgMax(registry);
    register_ArgMin(registry);
    register_Asin(registry);
    register_Asinh(registry);
    register_Atan(registry);
    register_Atanh(registry);
    register_Attention(registry);
    register_AveragePool(registry);
    register_BatchNormalization(registry);
    register_Bernoulli(registry);
    register_BitCast(registry);
    register_BitShift(registry);
    register_BitwiseAnd(registry);
    register_BitwiseNot(registry);
    register_BitwiseOr(registry);
    register_BitwiseXor(registry);
    register_BlackmanWindow(registry);
    register_Cast(registry);
    register_CastLike(registry);
    register_CausalConvWithState(registry);
    register_Ceil(registry);
    register_Celu(registry);
    register_CenterCropPad(registry);
    register_Clip(registry);
    register_Col2Im(registry);
    register_Compress(registry);
    register_Concat(registry);
    register_Constant(registry);
    register_ConstantOfShape(registry);
    register_Conv(registry);
    register_ConvInteger(registry);
    register_ConvTranspose(registry);
    register_Cos(registry);
    register_Cosh(registry);
    register_CumProd(registry);
    register_CumSum(registry);
    register_DFT(registry);
    register_DeformConv(registry);
    register_DepthToSpace(registry);
    register_DequantizeLinear(registry);
    register_Det(registry);
    register_Div(registry);
    register_Dropout(registry);
    register_DynamicQuantizeLinear(registry);
    register_Einsum(registry);
    register_Elu(registry);
    register_Equal(registry);
    register_Erf(registry);
    register_Exp(registry);
    register_Expand(registry);
    register_EyeLike(registry);
    register_Flatten(registry);
    register_Floor(registry);
    register_GRU(registry);
    register_Gather(registry);
    register_GatherElements(registry);
    register_GatherND(registry);
    register_Gelu(registry);
    register_Gemm(registry);
    register_GlobalAveragePool(registry);
    register_GlobalLpPool(registry);
    register_GlobalMaxPool(registry);
    register_Greater(registry);
    register_GreaterOrEqual(registry);
    register_GridSample(registry);
    register_GroupNormalization(registry);
    register_HammingWindow(registry);
    register_HannWindow(registry);
    register_HardSigmoid(registry);
    register_HardSwish(registry);
    register_Hardmax(registry);
    register_Identity(registry);
    register_ImageDecoder(registry);
    register_InstanceNormalization(registry);
    register_IsInf(registry);
    register_IsNaN(registry);
    register_LRN(registry);
    register_LSTM(registry);
    register_LayerNormalization(registry);
    register_LeakyRelu(registry);
    register_Less(registry);
    register_LessOrEqual(registry);
    register_LinearAttention(registry);
    register_Log(registry);
    register_LogSoftmax(registry);
    register_LpNormalization(registry);
    register_LpPool(registry);
    register_MatMul(registry);
    register_MatMulInteger(registry);
    register_Max(registry);
    register_MaxPool(registry);
    register_MaxRoiPool(registry);
    register_MaxUnpool(registry);
    register_Mean(registry);
    register_MeanVarianceNormalization(registry);
    register_MelWeightMatrix(registry);
    register_Min(registry);
    register_Mish(registry);
    register_Mod(registry);
    register_Mul(registry);
    register_Multinomial(registry);
    register_Neg(registry);
    register_NegativeLogLikelihoodLoss(registry);
    register_NonMaxSuppression(registry);
    register_NonZero(registry);
    register_Not(registry);
    register_OneHot(registry);
    register_OptionalGetElement(registry);
    register_OptionalHasElement(registry);
    register_Or(registry);
    register_PRelu(registry);
    register_Pad(registry);
    register_Pow(registry);
    register_QLinearConv(registry);
    register_QLinearMatMul(registry);
    register_QuantizeLinear(registry);
    register_RMSNormalization(registry);
    register_RNN(registry);
    register_RandomNormal(registry);
    register_RandomNormalLike(registry);
    register_RandomUniform(registry);
    register_RandomUniformLike(registry);
    register_Range(registry);
    register_Reciprocal(registry);
    register_ReduceL1(registry);
    register_ReduceL2(registry);
    register_ReduceLogSum(registry);
    register_ReduceLogSumExp(registry);
    register_ReduceMax(registry);
    register_ReduceMean(registry);
    register_ReduceMin(registry);
    register_ReduceProd(registry);
    register_ReduceSum(registry);
    register_ReduceSumSquare(registry);
    register_Relu(registry);
    register_Reshape(registry);
    register_Resize(registry);
    register_ReverseSequence(registry);
    register_RoiAlign(registry);
    register_RotaryEmbedding(registry);
    register_Round(registry);
    register_STFT(registry);
    register_Scatter(registry);
    register_ScatterElements(registry);
    register_ScatterND(registry);
    register_Selu(registry);
    register_Shape(registry);
    register_Shrink(registry);
    register_Sigmoid(registry);
    register_Sign(registry);
    register_Sin(registry);
    register_Sinh(registry);
    register_Size(registry);
    register_Slice(registry);
    register_Softmax(registry);
    register_SoftmaxCrossEntropyLoss(registry);
    register_Softplus(registry);
    register_Softsign(registry);
    register_SpaceToDepth(registry);
    register_Split(registry);
    register_Sqrt(registry);
    register_Squeeze(registry);
    register_Sub(registry);
    register_Sum(registry);
    register_Swish(registry);
    register_Tan(registry);
    register_Tanh(registry);
    register_TensorScatter(registry);
    register_TfIdfVectorizer(registry);
    register_ThresholdedRelu(registry);
    register_Tile(registry);
    register_TopK(registry);
    register_Transpose(registry);
    register_Trilu(registry);
    register_Unique(registry);
    register_Unsqueeze(registry);
    register_Upsample(registry);
    register_Where(registry);
    register_Xor(registry);
}

namespace
{
// Operators onnx defines but this table deliberately leaves out, so that the
// loader can say which of the two happened.
struct exclusion
{
    char const* domain;
    char const* optype;
    char const* reason;
};

constexpr exclusion k_exclusions[] = {
    {"", "ConcatFromSequence",
     "composite-type: 'S' admits only seq(tensor(bool)), seq(tensor(complex128)), seq(tensor(complex64)), seq(tensor(double)), and 11 more"},
    {"", "If",
     "subgraph: attribute 'else_branch' is required, attribute 'then_branch' is required"},
    {"", "Loop", "subgraph: attribute 'body' is required"},
    {"", "Optional",
     "composite-type: 'O' admits only optional(seq(tensor(bool))), optional(seq(tensor(complex128))), optional(seq(tensor(complex64))), optional(seq(tensor(double))), and 26 more"},
    {"", "RegexFullMatch", "no-encoding: 'T1' admits only tensor(string)"},
    {"", "Scan", "subgraph: attribute 'body' is required"},
    {"", "SequenceAt",
     "composite-type: 'S' admits only seq(tensor(bool)), seq(tensor(complex128)), seq(tensor(complex64)), seq(tensor(double)), and 11 more"},
    {"", "SequenceConstruct",
     "composite-type: 'S' admits only seq(tensor(bool)), seq(tensor(complex128)), seq(tensor(complex64)), seq(tensor(double)), and 11 more"},
    {"", "SequenceEmpty",
     "composite-type: 'S' admits only seq(tensor(bool)), seq(tensor(complex128)), seq(tensor(complex64)), seq(tensor(double)), and 11 more"},
    {"", "SequenceErase",
     "composite-type: 'S' admits only seq(tensor(bool)), seq(tensor(complex128)), seq(tensor(complex64)), seq(tensor(double)), and 11 more"},
    {"", "SequenceInsert",
     "composite-type: 'S' admits only seq(tensor(bool)), seq(tensor(complex128)), seq(tensor(complex64)), seq(tensor(double)), and 11 more"},
    {"", "SequenceLength",
     "composite-type: 'S' admits only seq(tensor(bool)), seq(tensor(complex128)), seq(tensor(complex64)), seq(tensor(double)), and 11 more"},
    {"", "SequenceMap", "subgraph: attribute 'body' is required"},
    {"", "SplitToSequence",
     "composite-type: 'S' admits only seq(tensor(bool)), seq(tensor(complex128)), seq(tensor(complex64)), seq(tensor(double)), and 11 more, 'S' admits only seq(tensor(bfloat16)), seq(tensor(bool)), seq(tensor(complex128)), seq(tensor(complex64)), and 12 more"},
    {"", "StringConcat", "no-encoding: 'T' admits only tensor(string)"},
    {"", "StringNormalizer",
     "no-encoding: operand 'X' is tensor(string), result 'Y' is tensor(string)"},
    {"", "StringSplit",
     "no-encoding: 'T1' admits only tensor(string), 'T2' admits only tensor(string)"},
};
} // namespace

char const*
onnx_schema_exclusion(std::string_view optype, std::string_view domain)
{
    for (auto const& entry : k_exclusions)
    {
        if (optype == entry.optype and domain == entry.domain)
        {
            return entry.reason;
        }
    }

    return nullptr;
}

} // namespace lnpu::nex::nx
