"""What to bake, and with what.

An entry is a design, not a shape -- but whether a design is shape-agnostic is a property of the
design, not something wished for, and the two operators here differ:

  gemm      The cores read their trip counts out of their own data memory, and the instruction
            stream writes them there. So the array does not depend on M, K or N. Measured: four
            shapes (K 1536/3072, N 512/2048/4096) produce a byte-identical PDI, and the streams
            for them carry 96 register writes apiece. One entry serves every projection.

  rmsnorm   No runtime parameters at all -- its streams carry zero register writes, so the tile
            size is compiled into the cores and the total size is tile times cores. A design
            handles exactly one (columns, channels, tile), and the catalog needs one entry per
            shape the model normalizes.

Comparing xclbins by hash does not show this, because two bakes of the same design differ (the
uuid is fresh each time) while the instruction streams are identical. The PDI is what to compare.

The descriptor each entry produces is written into the xclbin's USER_METADATA section, and the
runtime reads it back to build the matching amd::program. Nothing about a design is restated in
C++, because a value that disagreed with the xclbin it came from would fail nothing and merely
compute the wrong answer.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Any, Callable


@dataclass(frozen=True)
class Entry:
    """One design to bake."""

    name: str
    op: str
    build: Callable[[Any], Any]  # (AIEContext) -> an object with .compile() and .name
    descriptor: dict[str, Any]

    #: Columns the design spreads work over. Not the partition width -- see docs/08 8.4.
    columns: int = 8


def _rmsnorm(columns: int, channels: int, size: int, weighted: bool) -> Entry:
    """One design per shape. The tile is compiled into the cores; see the module docstring."""
    from iron.operators.rms_norm.op import RMSNorm

    tile = size // (columns * channels)
    kind = "w" if weighted else "u"

    return Entry(
        name=f"rmsnorm_{kind}_c{columns}_ch{channels}_t{tile}",
        op="rmsnorm",
        columns=columns,
        build=lambda ctx: RMSNorm(
            size=size,
            num_aie_columns=columns,
            num_channels=channels,
            tile_size=tile,
            weighted=weighted,
            context=ctx,
        ),
        descriptor={
            "op": "rmsnorm",
            "columns": columns,
            # Streams a column carries, which is also cores per column for the unweighted
            # design. Not rows: the weighted one runs two cores per channel, and where its
            # placer put them is not something this can state.
            "channels": channels,
            # What one core normalizes in one pass, and the total the design was baked for.
            # Both are here because neither follows from the other: the core loops
            # elements / (columns * channels * tile) times, and that count is a python-level
            # constant in the design, not a runtime parameter. So a design runs one size.
            "tile": tile,
            "elements": size,
            "weighted": weighted,
            "element": "bf16",
            "element_bytes": 2,
            "args": ["input", "weight", "output"] if weighted else ["input", "output"],
        },
    )


def _gemm(columns: int, tile_m: int, tile_k: int, tile_n: int, shape: tuple[int, int, int]) -> Entry:
    from iron.operators.gemm.op import GEMM

    m, k, n = shape

    return Entry(
        name=f"gemm_bf16_c{columns}_t{tile_m}x{tile_k}x{tile_n}",
        op="gemm",
        columns=columns,
        build=lambda ctx: GEMM(
            # A placeholder. IRON's constructor demands a shape, but the array it produces does
            # not depend on one: four different (K, N) bake to the same PDI byte for byte. What
            # decides the array is the tile and the column count, which is why the entry is named
            # after those and not after this.
            M=m,
            K=k,
            N=n,
            tile_m=tile_m,
            tile_k=tile_k,
            tile_n=tile_n,
            num_aie_columns=columns,
            b_col_maj=True,
            context=ctx,
        ),
        descriptor={
            "op": "gemm",
            "columns": columns,
            "rows": 4,
            "first_core_row": 2,
            "tile": {"m": tile_m, "k": tile_k, "n": tile_n},
            "element": "bf16",
            "element_bytes": 2,
            # Where a core reads its trip counts from, and what releases it. The compiler puts
            # the parameter buffer wherever it had room -- the same operator at a different tile
            # size lands elsewhere -- so bake.py fills these in from the stream it produced.
            "parameter_slots": None,
            "start_register": None,
            "args": ["a", "b", "c"],
        },
    )


def catalog() -> list[Entry]:
    return [
        # The three the hardware test already covers, so the baking path is checked against
        # artifacts whose numbers are known.
        _rmsnorm(columns=1, channels=1, size=2048, weighted=True),
        _rmsnorm(columns=8, channels=1, size=2048, weighted=False),
        _rmsnorm(columns=4, channels=1, size=4096, weighted=False),
        # One gemm design for every projection in the model. 32x64x64 on 8 columns is what a
        # 128-row prefill wants: M % (tile_m * 4) == 0 puts tile_m at 32, and 64 divides every K
        # and N except down_proj's 6144, which has to be split along K anyway because the stride
        # between B's column blocks passes what the descriptor field holds (docs/10 10.3).
        _gemm(columns=8, tile_m=32, tile_k=64, tile_n=64, shape=(128, 1536, 2048)),
    ]
