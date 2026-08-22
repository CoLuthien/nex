# AMD NPU 커널 합성

이 저장소가 쓰는 xclbin 을 굽는 곳. 산출물은 `lnpu-artifacts/amd/` 로 간다.

**CMake 밖에 있다.** xclbin 은 런타임 산출물이 아니라 에셋이고, `cmake --build` 가 파이썬
venv 를 만들고 aiecc 를 부르기 시작하면 빌드가 무거워지고 CI 가 아프다. 여기서 구운 결과를
저장소에 넣어 두고, 런타임은 그것을 읽기만 한다.

## 준비

```bash
./bootstrap.sh          # venv + 휠 + 툴체인 확인
```

필요한 것:

| | 어디서 | 비고 |
|---|---|---|
| `mlir_aie`, `llvm-aie` 휠 | `requirements.lock` (GitHub 릴리스 애셋) | PyPI 아님. nightly URL 이라 사라질 수 있다 |
| Python 3.10 / 3.12 / 3.13 | `uv` 가 받아 준다 | 3.14 는 휠이 없다 |
| `xclbinutil` | XRT | **없으면 aiecc 가 경고만 하고 exit 0 한다.** `bake.py` 가 산출물 존재를 직접 확인한다 |
| IRON | `submodules/iron` | 서브모듈. 설계 API 로 쓴다 |

NPU 는 필요 없다. 굽는 데는 하드웨어가 없어도 된다.

## 굽기

```bash
./run.sh bake --all                 # 카탈로그 전부
./run.sh bake --design rmsnorm      # 하나만
./run.sh bake --all --dry           # 커맨드만 출력
```

각 산출물은 xclbin 하나이고, **설계 기술자가 그 안 `USER_METADATA` 섹션에 들어 있다.**
사이드카 파일이 없으므로 xclbin 과 기술자가 어긋날 수가 없다. 런타임은 그것을 읽어
`amd::design` 을 채운다.

## 무엇이 어디에

| | |
|---|---|
| `designs/` | MLIR 을 만드는 파이썬. IRON 의 설계 API 를 쓴다 |
| `aie/` | 우리 AIE 커널 소스(`.cc`). peano 로 컴파일된다 |
| `catalog.py` | 무엇을 어떤 파라미터로 구울지 |
| `bake.py` | 설계 → xclbin → `USER_METADATA` 삽입 → `lnpu-artifacts/amd/` |
| `stubs/pyxrt.py` | NPU 없는 머신에서 IRON 을 import 하기 위한 것 |

## stubs/pyxrt.py 가 왜 있나

`iron/common/utils.py` 가 모듈 스코프에서 `pyxrt` 를 import 한다. 컴파일만 할 때도 걸린다.
실제로 쓰는 것은 버퍼 할당(하드웨어 실행)뿐이라, 최소 스텁으로 뚫는다. 스텁의 모든 호출은
`RuntimeError` 를 던진다 — 디바이스 호출이 조용히 no-op 되는 일이 없어야 한다.
