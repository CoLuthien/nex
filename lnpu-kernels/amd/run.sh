#!/usr/bin/env bash
# Runs a module in the baking venv with everything it needs on the path.
set -euo pipefail

here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
root="$(cd "$here/../.." && pwd)"
venv="$here/.venv"

[[ -x "$venv/bin/python" ]] || { echo "run ./bootstrap.sh first" >&2; exit 1; }

what="${1:?usage: run.sh <bake|...> [args]}"
shift

# stubs first: iron imports pyxrt at module scope and this machine may have no XRT for python.
# Exported rather than prefixed, because a prefix on a compound command lands on the builtin.
export PYTHONPATH="$here/stubs:$root/submodules/iron${PYTHONPATH:+:$PYTHONPATH}"

cd "$here"
exec "$venv/bin/python" "$here/$what.py" "$@"
