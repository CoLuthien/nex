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
./run.sh bake --all --fresh         # build/ 를 지우고 시작
```

**설계를 고쳤는데 결과가 그대로면 `--fresh` 부터 의심할 것.** IRON 은 산출물을 파일명으로
캐시해서, 이름이 같으면 다시 짓지 않는다. 이걸로 한 번 속았다 — 고친 설계를 구웠는데 이전
xclbin 이 그대로 나왔다.

각 산출물은 xclbin 하나이고, **설계 기술자가 그 안 `USER_METADATA` 섹션에 들어 있다.**
사이드카 파일이 없으므로 xclbin 과 기술자가 어긋날 수가 없다. 런타임은 그것을 읽어
`amd::design` 을 채운다.

## 무엇이 어디에

| | |
|---|---|
| `designs/` | IRON 설계 중 우리가 고쳐 쓰는 것 (아래) |
| `catalog.py` | 무엇을 어떤 파라미터로 구울지 |
| `bake.py` | 설계 → xclbin → `USER_METADATA` 삽입 → `lnpu-artifacts/amd/` |
| `stubs/pyxrt.py` | NPU 없는 머신에서 IRON 을 import 하기 위한 것 |

## designs/ — 고쳐서 쓰는 설계

IRON 의 설계를 그대로 쓸 수 없는 것만 여기 들어온다. 파일은 **고친 부분 말고는 업스트림과
바이트가 같다** — `iron/operators/<op>/design.py` 와 diff 하면 우리가 바꾼 것만 나온다.
고친 자리는 `nex:` 로 표시한다.

### gemm-design.py — 워커 배리어

워커는 런타임 배리어를 `Acquire` 로 기다리는데, 이 하드웨어에서 그건 "lock 이 이 값인지 본다"
이지 "가져간다" 가 아니다. 런타임 시퀀스가 lock 을 1 로 세워 워커를 풀고, 아무도 되돌리지
않는다 — 설계의 다른 모든 lock 은 `AcquireGreaterEqual` + `Release` 짝인데 배리어만 그렇다.
그래서 배리어는 **맨 처음 한 번만** 워커를 붙잡는다.

같은 어레이에 두 번째 shape 가 올 때 드러난다. 워커는 계산을 마치고 `while(true)` 를 한 바퀴
돌아 배리어를 즉시 통과하고 (lock 이 여전히 1 이므로), **호스트가 다음 값을 쓰기 전에** 런타임
파라미터를 읽는다. 그리고 이전 실행의 반복 횟수를 쥔 채 입력 fifo 에서 멈춘다.

Strix 에서 측정: K=1536 다음에 온 K=2048 이 "32 타일 중 앞 24 개만 더한 값" 을 내놓는다 —
상대오차 0.0087, 정상 실행의 오차와 같은 크기다. 소비하지 않은 8 타일이 fifo 에 남아 그 뒤
모든 실행이 어긋나고, 회복되지 않는다. N 만 바뀌는 것은 살아남는다 (워커가 다음 바퀴에서
모자란 타일을 마저 만들고 피연산자 수지는 맞는다). K 는 안 된다 — 낡은 값이 하필 "얼마나
소비할지" 를 정하는 값이다.

`AcquireGreaterEqual` 은 가져간 만큼 빼므로 `set_lock` 한 번에 통과 한 번이다. 런타임 시퀀스가
파라미터를 먼저 쓰고 lock 을 세우니, 워커가 배리어를 지날 수 있는 모든 시점에서 파라미터는
최신이다.

**명령 스트림은 안 바뀐다.** 전부 코어 프로그램 안이라, 에미터의 정답지 스트림은 그대로
바이트 일치하고 xclbin 만 달라진다. 실제로 그렇게 나왔다 — 스트림 동일, PDI 해시만 변경.

rmsnorm 설계는 배리어도 RTP 도 쓰지 않아 해당 없다.

## stubs/pyxrt.py 가 왜 있나

`iron/common/utils.py` 가 모듈 스코프에서 `pyxrt` 를 import 한다. 컴파일만 할 때도 걸린다.
실제로 쓰는 것은 버퍼 할당(하드웨어 실행)뿐이라, 최소 스텁으로 뚫는다. 스텁의 모든 호출은
`RuntimeError` 를 던진다 — 디바이스 호출이 조용히 no-op 되는 일이 없어야 한다.
