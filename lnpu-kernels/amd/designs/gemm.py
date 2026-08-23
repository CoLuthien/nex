"""GEMM, built from our copy of the design rather than IRON's.

The difference is one lock action; see the note at the top of gemm-design.py. Everything else --
the parameters, the kernel objects, the runtime argument shapes -- is upstream's, taken by
delegation rather than restated, so this cannot drift from the operator it specialises.
"""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path

from iron.common import DesignGenerator, PythonGeneratedMLIRArtifact
from iron.operators.gemm.op import GEMM as UpstreamGEMM

#: Our copy of iron/operators/gemm/design.py, with the barrier fix.
DESIGN = Path(__file__).resolve().parent / "gemm-design.py"


@dataclass
class GEMM(UpstreamGEMM):
    """The upstream operator, generated from the design next door.

    Only the source file moves. The callback and every argument come from upstream's own
    artifact, so a change there arrives here without this file being touched -- and the
    dependency is rebuilt around the new path rather than patched onto the old one, which is
    what makes the build notice when our design changes.
    """

    def get_mlir_artifact(self):
        upstream = super().get_mlir_artifact()
        theirs = upstream.generator

        return PythonGeneratedMLIRArtifact(
            upstream.filename,
            DesignGenerator(DESIGN, theirs.fn_name, theirs.args, theirs.kwargs),
        )
