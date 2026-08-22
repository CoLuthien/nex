#!/usr/bin/env python3
"""Bakes the catalog into lnpu-artifacts/amd.

Each artifact is one xclbin with its design descriptor inside it, in the USER_METADATA section.
There is no sidecar, so the descriptor cannot drift from the xclbin it describes -- which matters
because a wrong tile size fails nothing and merely computes the wrong answer.
"""

from __future__ import annotations

import argparse
import json
import shutil
import struct
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
OUT = ROOT / "lnpu-artifacts" / "amd"

SCHEMA = 1

#: Transaction opcodes we need to read back out of a stream. See docs/01.
_WRITE, _BLOCKWRITE, _MASKWRITE, _TCT, _DDR_PATCH = 0, 1, 3, 0x80, 0x81
_SIZE_WORD_AT = {_WRITE: 5, _BLOCKWRITE: 3, _MASKWRITE: 6, _TCT: 1, _DDR_PATCH: 1}

#: Shim task-queue registers, which a plain write also targets. Anything outside this and the
#: core-release register is a runtime parameter.
_QUEUE_LOW, _QUEUE_HIGH = 0x1D200, 0x1D21F
_START_REGISTER = 0x1F000


def _commands(stream: bytes):
    """Walks a transaction stream, yielding (opcode, words)."""
    words = list(struct.unpack(f"<{len(stream) // 4}I", stream))
    at = 4  # past the four header words
    while at < len(words):
        opcode = words[at]
        if opcode not in _SIZE_WORD_AT:
            raise RuntimeError(f"unknown opcode 0x{opcode:02x} at word {at}")
        length = words[at + _SIZE_WORD_AT[opcode]] // 4
        if length <= 0:
            raise RuntimeError(f"opcode 0x{opcode:02x} at word {at} claims {length} words")
        yield opcode, words[at : at + length]
        at += length


def runtime_parameters(stream: bytes) -> tuple[list[int], int | None]:
    """Reads where a design keeps its trip counts, and what releases its cores.

    Neither follows from the operation -- the compiler puts the parameter buffer wherever the
    core had room, and the same operator at a different tile size lands somewhere else. Reading
    it back off the stream the compiler just produced is what keeps the descriptor honest.
    """
    slots: list[int] = []
    start: int | None = None

    for opcode, words in _commands(stream):
        if opcode != _WRITE:
            continue
        address = words[2] & 0x1FFFF
        if _QUEUE_LOW <= address <= _QUEUE_HIGH:
            continue
        if address == _START_REGISTER:
            start = address
        elif address not in slots:
            slots.append(address)

    return sorted(slots), start


def embed(xclbin: Path, descriptor: dict, into: Path) -> None:
    """Puts the descriptor in the xclbin's USER_METADATA section."""
    payload = into.with_suffix(".design.json")
    payload.write_text(json.dumps(descriptor, indent=1))

    subprocess.run(
        ["xclbinutil", "--input", str(xclbin),
         "--add-section", f"USER_METADATA:RAW:{payload}",
         "--output", str(into), "--force"],
        check=True, capture_output=True,
    )
    payload.unlink()


def bake(entry, build_dir: Path, dry: bool) -> bool:
    from iron.common.context import AIEContext

    print(f"\n=== {entry.name}")

    operator = entry.build(AIEContext(build_dir=str(build_dir)))
    print(f"    design  {operator.name}")

    operator.compile(dry_run=dry)
    if dry:
        return True

    # aiecc warns and exits 0 when xclbinutil is missing, and IRON marks the artifact available
    # on a zero exit. Nothing but looking catches that.
    xclbin = build_dir / f"{operator.name}.xclbin"
    stream = build_dir / f"{operator.name}.bin"
    for produced in (xclbin, stream):
        if not produced.exists():
            print(f"    빠짐: {produced.name} -- aiecc 가 성공했다고 했지만 만들지 않았다")
            return False

    descriptor = dict(entry.descriptor)
    descriptor["schema"] = SCHEMA
    descriptor["generation"] = "npu2"
    descriptor["partition_columns"] = 8

    if "parameter_slots" in descriptor and descriptor["parameter_slots"] is None:
        slots, start = runtime_parameters(stream.read_bytes())
        descriptor["parameter_slots"] = [f"0x{slot:05x}" for slot in slots]
        descriptor["start_register"] = f"0x{start:05x}" if start is not None else None
        print(f"    runtime parameters  slots={descriptor['parameter_slots']} "
              f"start={descriptor['start_register']}")

    OUT.mkdir(parents=True, exist_ok=True)
    into = OUT / f"{entry.name}.xclbin"
    embed(xclbin, descriptor, into)

    # The reference stream ships beside it: the emitter is checked against it before anything is
    # sent to hardware, and a mismatch means there is nothing for the hardware to say.
    shutil.copy(stream, OUT / f"{entry.name}.aiecc.bin")

    print(f"    {into.name}  {into.stat().st_size} B  "
          f"(+{OUT.joinpath(entry.name + '.aiecc.bin').stat().st_size} B stream)")
    return True


def main() -> int:
    from aie.iron.device import NPU2
    from aie.utils import set_current_device

    parsed = argparse.ArgumentParser(description=__doc__)
    parsed.add_argument("--design", action="append", help="이름 하나만 (여러 번 가능)")
    parsed.add_argument("--all", action="store_true")
    parsed.add_argument("--dry", action="store_true", help="커맨드만 출력")
    parsed.add_argument("--build-dir", default=str(HERE / "build"))
    args = parsed.parse_args()

    # There is no NPU here to ask, so the target is stated.
    set_current_device(NPU2())

    from catalog import catalog

    entries = catalog()
    if args.design:
        wanted = set(args.design)
        entries = [one for one in entries if one.name in wanted]
        if missing := wanted - {one.name for one in entries}:
            print(f"그런 설계 없음: {', '.join(sorted(missing))}", file=sys.stderr)
            return 2
    elif not args.all:
        print("무엇을 구울지 --design 또는 --all 로 말할 것", file=sys.stderr)
        print("\n카탈로그:", file=sys.stderr)
        for one in entries:
            print(f"  {one.name:36s} {one.op}", file=sys.stderr)
        return 2

    build_dir = Path(args.build_dir)
    build_dir.mkdir(parents=True, exist_ok=True)

    baked = sum(bake(one, build_dir, args.dry) for one in entries)
    print(f"\n{len(entries)}개 중 {baked}개")
    return 0 if baked == len(entries) else 1


if __name__ == "__main__":
    sys.exit(main())
