#!/usr/bin/env python3
"""Emit the C++ ONNX operator schema table used by lnpu::nex::nx::schema_registry.

The onnx python package already carries every operator signature it ever
published (onnx.defs.get_all_schemas_with_history()).  This script projects that
data onto the types this runtime can actually represent and writes:

  op-schemas.gen.cpp      register_onnx_schemas(schema_registry&) plus
                          onnx_schema_exclusion(optype, domain), which lets the
                          loader tell "deliberately not generated" apart from
                          "never heard of it".
  op-schemas-coverage.md  what was dropped or narrowed on the way, so that a
                          regeneration against a newer onnx package shows up as
                          a reviewable diff.

Both outputs are deterministic: sorted, unversioned, no timestamps.

Projection rules
  * dtypes fold onto EDataType (cmn/numeric-types.hpp).  The fold is lossy --
    int64/uint64/uint32 all land on i32, double on f32, bool on u8 -- matching
    what onnx-decode.cpp does to the payload.  Duplicates collapse.
  * a dtype with no encoding at all (string, complex, float8/4, int2/uint2)
    simply leaves the constraint; a constraint left empty drops the version.
  * a required graph attribute (If/Loop/Scan) drops the op: this frontend has
    no notion of a subgraph.
  * a required attribute of any other unrepresentable type drops the op; an
    optional one is merely omitted.
  * deprecated versions are never emitted.
  * an op is emitted only if the versions that survive form a suffix of its
    history.  schema_registry::find() floor-matches, so a hole in the middle
    would silently answer with an older, wrong signature -- no answer is safer.

Usage
  ./generate-op-schemas.py [--output-dir DIR] [--domain DOMAIN]... [--verify]
                           [--explain OP]
"""

from __future__ import annotations

import argparse
import math
import sys
from pathlib import Path

try:
    import onnx
    from onnx import AttributeProto, defs
    from onnx.defs import OpSchema
except ImportError:  # pragma: no cover - environment problem, not a code path
    sys.stderr.write("error: this script needs the 'onnx' python package\n")
    sys.exit(1)

SOURCE_NAME = "op-schemas.gen.cpp"
REPORT_NAME = "op-schemas-coverage.md"

COLUMNS = 100

# how far the members of a layer_schema initializer sit from the left margin
NESTING = 30

# onnx type string -> EDataType enumerator.  Several onnx dtypes share one
# enumerator; ENCODING_ORDER keeps the emitted lists in enum order so that the
# fold stays visible as a shrinking list rather than a shuffled one.
ENCODING_OF = {
    "tensor(int8)": "i8",
    "tensor(uint8)": "u8",
    "tensor(bool)": "u8",
    "tensor(int16)": "i16",
    "tensor(uint16)": "u16",
    "tensor(int32)": "i32",
    "tensor(uint32)": "i32",
    "tensor(int64)": "i32",
    "tensor(uint64)": "i32",
    "tensor(float16)": "f16",
    "tensor(bfloat16)": "bf16",
    "tensor(float)": "f32",
    "tensor(double)": "f32",
}

ENCODING_ORDER = ["i8", "u8", "i16", "u16", "i32", "f16", "bf16", "f32"]

ATTRIBUTE_OF = {
    OpSchema.AttrType.FLOAT: "f32",
    OpSchema.AttrType.INT: "i64",
    OpSchema.AttrType.STRING: "str",
    OpSchema.AttrType.FLOATS: "f32s",
    OpSchema.AttrType.INTS: "i64s",
    OpSchema.AttrType.STRINGS: "strs",
    OpSchema.AttrType.TENSOR: "tensor",
}

ARITY_OF = {
    OpSchema.FormalParameterOption.Single: None,  # EArity::single, the default
    OpSchema.FormalParameterOption.Optional: "EArity::optional",
    OpSchema.FormalParameterOption.Variadic: "EArity::variadic",
}

SUBGRAPH_ATTRIBUTES = {OpSchema.AttrType.GRAPH, OpSchema.AttrType.GRAPHS}

# Exclusion reasons, most specific first: a version that hits several is filed
# under the first one it hits.
EXCLUSIONS = [
    ("subgraph", "subgraph-valued attribute (control flow)"),
    ("composite-type", "sequence / map / optional / sparse type"),
    ("no-encoding", "no representable element type"),
    ("attribute-type", "unrepresentable required attribute"),
    ("experimental", "experimental support level"),
    ("version-hole", "unrepresentable version between representable ones"),
    ("deprecated", "every version deprecated"),
]

AMENDMENTS = [
    ("folded", "dtypes folded onto fewer encodings"),
    ("dropped-dtype", "dtypes with no encoding removed"),
    ("dropped-attribute", "optional attributes omitted"),
    ("dropped-default", "attribute defaults omitted"),
    ("dropped-version", "leading versions omitted"),
    ("dropped-deprecated", "deprecated versions omitted"),
]


# --------------------------------------------------------------------------- #
# rendering helpers
# --------------------------------------------------------------------------- #


def canonical_domain(domain: str) -> str:
    """"" and "ai.onnx" name the same (default) domain; "" is how we store it."""
    return "" if domain in ("", "ai.onnx") else domain


def spell_domain(domain: str) -> str:
    return domain or '"" (ai.onnx)'


def spell_op(domain: str, optype: str) -> str:
    return f"{domain}:{optype}" if domain else optype


def quoted(text) -> str:
    """A C++ string literal holding `text`, escaped down to plain ASCII."""
    if isinstance(text, (bytes, bytearray)):
        text = text.decode("utf-8")

    pieces = []
    for character in text:
        code = ord(character)
        if character in ('"', "\\"):
            pieces.append("\\" + character)
        elif character == "\n":
            pieces.append("\\n")
        elif character == "\t":
            pieces.append("\\t")
        elif character == "\r":
            pieces.append("\\r")
        elif 0x20 <= code < 0x7F:
            pieces.append(character)
        else:
            # break the literal so that a following hex digit cannot be eaten
            # by the escape
            pieces.append(f'\\x{code:02x}" "')
    return '"' + "".join(pieces) + '"'


def float_literal(value: float) -> str:
    if math.isnan(value):
        return "std::numeric_limits<float>::quiet_NaN()"
    if math.isinf(value):
        return ("-" if value < 0 else "") + "std::numeric_limits<float>::infinity()"
    return f"{value!r}f"


def mangled(text: str) -> str:
    return "".join(character if character.isalnum() else "_" for character in text)


def filled(items: list, opening: str, closing: str, budget: int = COLUMNS) -> list:
    """`items` laid out after `opening`, wrapped at `budget`, aligned underneath."""
    indent = " " * len(opening)
    lines = []
    current = opening

    for position, item in enumerate(items):
        last = position + 1 == len(items)
        piece = item + ("" if last else ",")
        fresh = current.endswith(" ") or current in (opening, indent)
        candidate = current + piece if fresh else current + " " + piece
        # the last line still has to fit the closer that gets appended to it
        room = budget - (len(closing) if last else 0)

        if len(candidate) > room and not (current in (opening, indent)):
            lines.append(current)
            current = indent + piece
        else:
            current = candidate

    lines.append(current + closing)
    return lines


def shortened(items: list, keep: int = 4) -> str:
    if len(items) <= keep:
        return ", ".join(items)
    return ", ".join(items[:keep]) + f", and {len(items) - keep} more"


# --------------------------------------------------------------------------- #
# one opset version of one operator
# --------------------------------------------------------------------------- #


class Version:
    """The projection of a single OpSchema onto our own vocabulary.

    `blocks` non-empty means the version is not emitted; `amendments` records
    what had to change for the ones that are.  Both are (kind, detail) pairs.
    """

    def __init__(self, schema):
        self.schema = schema
        self.opset = schema.since_version
        self.blocks = []
        self.amendments = []
        self.text = None

        self._constraints = []  # [(name, [encoding])] in schema order
        self._declared = set()  # constraint names the schema itself declares
        self._attributes = []
        self._operands = []
        self._results = []

    # -- projection --------------------------------------------------------- #

    def project(self) -> "Version":
        if self.schema.support_level != OpSchema.SupportType.COMMON:
            level = str(self.schema.support_level).rsplit(".", 1)[-1]
            self._block("experimental", f"support level is {level}")

        self._project_attributes()
        self._project_constraints()
        self._operands = self._project_parameters(self.schema.inputs, "operand")
        self._results = self._project_parameters(self.schema.outputs, "result")

        if not self.blocks:
            self.text = self._render()
        return self

    def _block(self, kind: str, detail: str) -> None:
        self.blocks.append((kind, detail))

    def _amend(self, kind: str, detail: str) -> None:
        self.amendments.append((kind, detail))

    def _project_attributes(self) -> None:
        # onnx attributes are named, not positional, so the order is ours to pick
        for name in sorted(self.schema.attributes):
            attribute = self.schema.attributes[name]
            kind = ATTRIBUTE_OF.get(attribute.type)

            if kind is None:
                spelling = str(attribute.type).rsplit(".", 1)[-1]
                if not attribute.required:
                    self._amend("dropped-attribute", f"{name} of type {spelling}")
                elif attribute.type in SUBGRAPH_ATTRIBUTES:
                    self._block("subgraph", f"attribute '{name}' is required")
                else:
                    self._block("attribute-type", f"required '{name}' is of type {spelling}")
                continue

            fields = [
                f".required = {'true' if attribute.required else 'false'}",
                f".name = {quoted(name)}",
                f".type = EAttributeType::{kind}",
            ]
            default = self._default_of(attribute.default_value, name)
            if default is not None:
                fields.append(f".default_value = {default}")

            self._attributes.extend(filled(fields, "{", "},", COLUMNS - NESTING))

    def _default_of(self, default, name: str):
        """The default as an attribute_value initializer, or None when absent."""
        kind = default.type

        if kind == AttributeProto.UNDEFINED:
            return None
        if kind == AttributeProto.FLOAT:
            return f"attribute_value{{{float_literal(default.f)}}}"
        if kind == AttributeProto.INT:
            return f"attribute_value{{std::int64_t{{{default.i}}}}}"
        if kind == AttributeProto.STRING:
            return f"attribute_value{{std::string{{{quoted(default.s)}}}}}"
        if kind == AttributeProto.FLOATS:
            values = ", ".join(float_literal(value) for value in default.floats)
            return f"attribute_value{{std::vector<float>{{{values}}}}}"
        if kind == AttributeProto.INTS:
            values = ", ".join(f"std::int64_t{{{value}}}" for value in default.ints)
            return f"attribute_value{{std::vector<std::int64_t>{{{values}}}}}"
        if kind == AttributeProto.STRINGS:
            values = ", ".join(f"std::string{{{quoted(value)}}}" for value in default.strings)
            return f"attribute_value{{std::vector<std::string>{{{values}}}}}"

        # a tensor default has to be built, not spelled; the attribute stays,
        # its default does not
        self._amend("dropped-default", name)
        return None

    def _project_constraints(self) -> None:
        for constraint in self.schema.type_constraints:
            name = constraint.type_param_str
            self._declared.add(name)

            encodings = []
            unrepresentable = []
            for spelling in constraint.allowed_type_strs:
                encoding = ENCODING_OF.get(spelling)
                if encoding is None:
                    unrepresentable.append(spelling)
                elif encoding not in encodings:
                    encodings.append(encoding)

            if not encodings:
                composite = [t for t in unrepresentable if not t.startswith("tensor(")]
                kind = "composite-type" if composite else "no-encoding"
                self._block(kind, f"'{name}' admits only {shortened(sorted(unrepresentable))}")
                continue

            if unrepresentable:
                self._amend("dropped-dtype", f"{name}: {', '.join(sorted(unrepresentable))}")

            folded = len([t for t in constraint.allowed_type_strs if t in ENCODING_OF])
            if folded > len(encodings):
                self._amend("folded", f"{name}: {folded} dtypes onto {len(encodings)} encodings")

            self._constraints.append((name, sorted(encodings, key=ENCODING_ORDER.index)))

    def _project_parameters(self, parameters, role: str) -> list:
        lines = []
        for parameter in parameters:
            spelling = parameter.type_str

            if spelling in self._declared:
                # the usual case: the parameter names a type constraint
                reference = spelling
            elif spelling in ENCODING_OF:
                # a hard-wired dtype instead (Reshape's "shape" is tensor(int64));
                # give it a constraint of its own, named after the dtype
                reference = spelling
                if reference not in (name for name, _ in self._constraints):
                    self._constraints.append((reference, [ENCODING_OF[spelling]]))
            elif spelling.startswith("tensor("):
                self._block("no-encoding", f"{role} '{parameter.name}' is {spelling}")
                continue
            else:
                self._block("composite-type", f"{role} '{parameter.name}' is {spelling}")
                continue

            fields = [f".name = {quoted(parameter.name)}", f".type_param = {quoted(reference)}"]
            arity = ARITY_OF[parameter.option]
            if arity is not None:
                fields.append(f".arity = {arity}")

            lines.extend(filled(fields, "{", "},", COLUMNS - NESTING))
        return lines

    # -- emission ----------------------------------------------------------- #

    def _render(self) -> str:
        out = [
            f"    r.register_schema({self.opset},",
            "                      layer_schema{",
            f"                          .domain = {quoted(canonical_domain(self.schema.domain))},",
            f"                          .optype = {quoted(self.schema.name)},",
        ]
        pad = " " * (NESTING - 4)

        def sequence(field: str, lines: list) -> None:
            if not lines:
                out.append(f"{pad}.{field} = {{}},")
                return
            out.append(f"{pad}.{field} = {{")
            out.extend(f"{pad}    {line}" for line in lines)
            out.append(f"{pad}}},")

        sequence("operands", self._operands)
        sequence("results", self._results)

        if self._constraints:
            out.append(f"{pad}.type_constraints = {{")
            for name, encodings in self._constraints:
                spelled = [f"EDataType::{encoding}" for encoding in encodings]
                opening = f"{pad}    {{.name = {quoted(name)}, .allowed = {{"
                out.extend(filled(spelled, opening, "}},"))
            out.append(f"{pad}}},")
        else:
            out.append(f"{pad}.type_constraints = {{}},")

        sequence("attrs", self._attributes)

        out.append(" " * (NESTING - 8) + "});")
        return "\n".join(out)


# --------------------------------------------------------------------------- #
# one operator, across its whole history
# --------------------------------------------------------------------------- #


class Operator:
    def __init__(self, domain: str, optype: str):
        self.domain = domain
        self.optype = optype
        self.kept = []
        self.lost = []
        self.deprecated = []
        self.extra = []  # op-level amendments: (kind, detail, [opset])
        self.exclusion = None  # (kind, detail) once settled, None when emitted

    def __repr__(self) -> str:
        return f"Operator({self.label!r}, {'excluded' if self.exclusion else 'emitted'})"

    @property
    def label(self) -> str:
        return spell_op(self.domain, self.optype)

    @property
    def function(self) -> str:
        stem = mangled(f"{self.domain}_{self.optype}") if self.domain else mangled(self.optype)
        return f"register_{stem}"

    def consider(self, schema) -> None:
        if schema.deprecated:
            self.deprecated.append(schema.since_version)
            return
        version = Version(schema).project()
        (self.kept if not version.blocks else self.lost).append(version)

    def settle(self) -> None:
        """Decide whether the operator makes it into the table."""
        if self.kept and self.lost:
            if max(v.opset for v in self.lost) < min(v.opset for v in self.kept):
                # only a leading run of old versions is unrepresentable, which
                # find() answers with nullptr rather than a wrong signature
                for version in self.lost:
                    kind, detail = version.blocks[0]
                    self.extra.append(("dropped-version", f"{kind}: {detail}", [version.opset]))
                self.lost = []
                return

            kind, detail = self.lost[0].blocks[0]
            self.exclusion = (
                "version-hole",
                "opset {} representable, {} not ({}: {})".format(
                    ", ".join(str(v.opset) for v in self.kept),
                    ", ".join(str(v.opset) for v in self.lost),
                    kind,
                    detail,
                ),
            )
            self.kept = []
            return

        if self.lost:
            reasons = [block for version in self.lost for block in version.blocks]
            kind = min((kind for kind, _ in reasons), key=[k for k, _ in EXCLUSIONS].index)
            details = list(dict.fromkeys(detail for k, detail in reasons if k == kind))
            self.exclusion = (kind, shortened(details))
            return

        if not self.kept and self.deprecated:
            self.exclusion = ("deprecated", "no version survives deprecation")

    def explain(self) -> str:
        out = [f"operator {self.label}"]
        if self.exclusion:
            out.append("  excluded ({}: {})".format(*self.exclusion))
        else:
            out.append(f"  emitted, {len(self.kept)} version(s)")
        if self.deprecated:
            out.append("  deprecated: " + ", ".join(str(v) for v in self.deprecated))
        for kind, detail, opsets in self.extra:
            out.append(f"  [{kind}] opset {', '.join(str(v) for v in opsets)}: {detail}")
        for version in sorted(self.kept + self.lost, key=lambda v: v.opset):
            out.append(f"  opset {version.opset}: {'kept' if not version.blocks else 'lost'}")
            for kind, detail in version.blocks:
                out.append(f"    blocked [{kind}] {detail}")
            for kind, detail in version.amendments:
                out.append(f"    amended [{kind}] {detail}")
            if version.text:
                out.extend("    | " + line for line in version.text.splitlines())
        return "\n".join(out)


def collect(domains: list) -> tuple:
    """Every operator of `domains`, projected and settled, sorted by name."""
    history = {}
    for schema in defs.get_all_schemas_with_history():
        domain = canonical_domain(schema.domain)
        if domain in domains:
            history.setdefault((domain, schema.name), {}).setdefault(schema.since_version, schema)

    operators = []
    for key in sorted(history):
        operator = Operator(*key)
        for opset in sorted(history[key]):
            operator.consider(history[key][opset])
        operator.settle()
        operators.append(operator)

    about = {
        "onnx": onnx.__version__,
        "opset": defs.onnx_opset_version(),
        "domains": ", ".join(spell_domain(domain) for domain in domains),
    }
    return operators, about


# --------------------------------------------------------------------------- #
# the two output files
# --------------------------------------------------------------------------- #


def render_source(operators: list, about: dict) -> str:
    emitted = [operator for operator in operators if operator.kept]
    excluded = [operator for operator in operators if operator.exclusion]

    body = "\n".join(version.text for op in emitted for version in op.kept)
    includes = ["#include <cstdint>", "#include <string_view>"]
    if "numeric_limits" in body:
        includes.append("#include <limits>")

    out = [
        f"// Written by {Path(__file__).name}. Regenerate rather than edit.",
        "//",
        f"// onnx package : {about['onnx']}",
        f"// newest opset : {about['opset']}",
        f"// domains      : {about['domains']}",
        "//",
        f"// What this table leaves out, and why, is listed in {REPORT_NAME}.",
        "",
        '#include "schema-registry.hpp"',
        "",
    ]
    out.extend(sorted(includes))
    out += ["", "namespace lnpu::nex::nx", "{", ""]

    for operator in emitted:
        out += ["static void", f"{operator.function}(schema_registry& r)", "{"]
        out.append("\n\n".join(version.text for version in operator.kept))
        out += ["}", ""]

    out += ["void", "register_onnx_schemas(schema_registry& registry)", "{"]
    out += [f"    {operator.function}(registry);" for operator in emitted]
    out += ["}", ""]

    out += [
        "namespace",
        "{",
        "// Operators onnx defines but this table deliberately leaves out, so that the",
        "// loader can say which of the two happened.",
        "struct exclusion",
        "{",
        "    char const* domain;",
        "    char const* optype;",
        "    char const* reason;",
        "};",
        "",
        "constexpr exclusion k_exclusions[] = {",
    ]
    for operator in excluded:
        reason = "{}: {}".format(*operator.exclusion)
        out.extend(
            filled(
                [quoted(operator.domain), quoted(operator.optype), quoted(reason)],
                "    {",
                "},",
            )
        )
    out += [
        "};",
        "} // namespace",
        "",
        "char const*",
        "onnx_schema_exclusion(std::string_view optype, std::string_view domain)",
        "{",
        "    for (auto const& entry : k_exclusions)",
        "    {",
        "        if (optype == entry.optype and domain == entry.domain)",
        "        {",
        "            return entry.reason;",
        "        }",
        "    }",
        "",
        "    return nullptr;",
        "}",
        "",
        "} // namespace lnpu::nex::nx",
        "",
    ]
    return "\n".join(out)


def render_report(operators: list, about: dict) -> str:
    emitted = [operator for operator in operators if operator.kept]
    excluded = [operator for operator in operators if operator.exclusion]

    out = [
        "# ONNX operator coverage",
        "",
        f"Written by {Path(__file__).name} together with {SOURCE_NAME}; regenerate both",
        "at once and review the diff.",
        "",
        f"- onnx package: {about['onnx']}",
        f"- newest opset: {about['opset']}",
        f"- domains: {about['domains']}",
        "- emitted: {} versions over {} operators".format(
            sum(len(operator.kept) for operator in emitted), len(emitted)
        ),
        f"- excluded: {len(excluded)} operators",
        "",
        "## Excluded operators",
    ]

    if not excluded:
        out += ["", "(none)"]

    for kind, title in EXCLUSIONS:
        entries = [operator for operator in excluded if operator.exclusion[0] == kind]
        if not entries:
            continue
        out += ["", f"### {title}", ""]
        for operator in entries:
            opsets = sorted({v.opset for v in operator.lost} | set(operator.deprecated))
            listed = ", ".join(str(opset) for opset in opsets)
            where = f" (opset {listed})" if listed else ""
            out.append(f"- `{operator.label}`{where}: {operator.exclusion[1]}")

    grouped = {}
    for operator in emitted:
        per_amendment = {}
        for version in operator.kept:
            for amendment in version.amendments:
                per_amendment.setdefault(amendment, []).append(version.opset)
        for (kind, detail), opsets in per_amendment.items():
            grouped.setdefault(kind, []).append((operator.label, detail, opsets))
        for kind, detail, opsets in operator.extra:
            grouped.setdefault(kind, []).append((operator.label, detail, opsets))
        if operator.deprecated:
            grouped.setdefault("dropped-deprecated", []).append(
                (operator.label, "", sorted(operator.deprecated))
            )

    out += ["", "## Emitted with amendments"]
    if not grouped:
        out += ["", "(none)"]

    for kind, title in AMENDMENTS:
        entries = grouped.get(kind)
        if not entries:
            continue
        out += ["", f"### {title}", ""]
        for label, detail, opsets in sorted(entries):
            listed = ", ".join(str(opset) for opset in opsets)
            out.append(f"- `{label}` (opset {listed}){': ' + detail if detail else ''}")

    out.append("")
    return "\n".join(out)


# --------------------------------------------------------------------------- #
# entry point
# --------------------------------------------------------------------------- #


def main() -> int:
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        "--output-dir",
        type=Path,
        default=Path(__file__).resolve().parent,
        help="where to write the generated files (default: next to this script)",
    )
    parser.add_argument(
        "--domain",
        action="append",
        dest="domains",
        metavar="DOMAIN",
        help="onnx domain to cover, repeatable (default: ai.onnx)",
    )
    parser.add_argument(
        "--verify",
        action="store_true",
        help="write nothing; exit 2 when the files on disk are stale (for CI)",
    )
    parser.add_argument(
        "--explain",
        metavar="OP",
        help="print how one operator ('OpType' or 'domain:OpType') was projected, then stop",
    )
    arguments = parser.parse_args()

    domains = sorted({canonical_domain(d) for d in (arguments.domains or ["ai.onnx"])})
    operators, about = collect(domains)

    if arguments.explain:
        domain, _, optype = arguments.explain.rpartition(":")
        wanted = (canonical_domain(domain), optype)
        for operator in operators:
            if (operator.domain, operator.optype) == wanted:
                print(operator.explain())
                return 0
        sys.stderr.write(f"error: no such operator: {spell_op(*wanted)}\n")
        return 1

    files = {
        arguments.output_dir / SOURCE_NAME: render_source(operators, about),
        arguments.output_dir / REPORT_NAME: render_report(operators, about),
    }

    stale = 0
    if arguments.verify:
        for path, text in files.items():
            if not path.exists() or path.read_text(encoding="utf-8") != text:
                sys.stderr.write(f"stale: {path}\n")
                stale = 2
        if not stale:
            print("up to date")
    else:
        arguments.output_dir.mkdir(parents=True, exist_ok=True)
        for path, text in files.items():
            path.write_text(text, encoding="utf-8")
            print(f"wrote {path}")

    emitted = [operator for operator in operators if operator.kept]
    excluded = [operator for operator in operators if operator.exclusion]
    print(
        f"{sum(len(operator.kept) for operator in emitted)} versions "
        f"over {len(emitted)} operators; {len(excluded)} operators excluded"
    )
    return stale


if __name__ == "__main__":
    sys.exit(main())
