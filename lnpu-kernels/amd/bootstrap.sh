#!/usr/bin/env bash
# Sets up the toolchain that bakes xclbins. No NPU needed -- compiling does not touch one.
set -euo pipefail

here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
root="$(cd "$here/../.." && pwd)"
iron="$root/submodules/iron"
venv="$here/.venv"

if [[ ! -f "$iron/pyproject.toml" ]]; then
    echo "submodules/iron is empty. git submodule update --init --recursive" >&2
    exit 1
fi

# The wheels exist for 3.10, 3.12 and 3.13 only; a distro python is often newer than that.
python="${PYTHON:-3.12}"

if ! command -v uv >/dev/null; then
    echo "uv not found. It is what fetches a python the wheels exist for." >&2
    exit 1
fi

echo "==> venv (python $python)"
uv venv --python "$python" "$venv"

echo "==> wheels"
# uv gathers candidates from the first index only unless told otherwise, and torch's +cpu build
# lives on a different one than the rest.
VIRTUAL_ENV="$venv" uv pip install --index-strategy unsafe-best-match -r "$here/requirements.lock"

echo "==> iron (editable, from the submodule)"
VIRTUAL_ENV="$venv" uv pip install --no-deps -e "$iron"

echo "==> tools"
missing=0
for tool in xclbinutil; do
    if command -v "$tool" >/dev/null; then
        echo "    $tool: $(command -v "$tool")"
    else
        # aiecc warns and exits 0 without this, leaving no xclbin and a successful build.
        echo "    $tool: NOT FOUND -- aiecc will skip xclbin generation and still succeed" >&2
        missing=1
    fi
done

aiecc="$("$venv/bin/python" -c 'import aie.utils.config as c; print(c.root_path())')/bin/aiecc"
[[ -x "$aiecc" ]] && echo "    aiecc: $aiecc" || { echo "    aiecc: NOT FOUND at $aiecc" >&2; missing=1; }

echo
if (( missing )); then
    echo "준비 미완: 위의 도구를 갖춘 뒤 다시 실행할 것" >&2
    exit 1
fi
echo "준비 완료.  ./run.sh bake --all"
