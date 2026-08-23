# NPU 하드웨어 테스트

NPU가 달린 장비에서 **이 저장소의 백엔드가 실제로 도는지, 그리고 결과가 맞는지** 확인한다.

XRT 를 직접 부르지 않는다. 스트림은 `amd::program` 이 만들고, 어레이는 `amd::device` 가 열고,
실행은 `amd::operation` 을 거친다 — 런타임을 이루는 바로 그 객체들이다. 이걸 우회하면 정확히
놓치기 쉬운 것을 놓친다: `device` 에는 생성자 정의조차 없었는데, 아무도 만들어 본 적이 없어서
아무도 몰랐다.

**케이스는 나열하지 않고 발견한다.** 디렉터리의 xclbin 을 하나씩 열어 기술자에게 어떤
오퍼레이터인지 묻고, 옆에 놓인 aiecc 스트림들이 어떤 shape 를 돌릴지 말해 준다 — 스트림 하나가
케이스 하나다. 그래서 설계를 하나 구우면 케이스가 따라 늘고, 손으로 맞춰 둘 목록이 없다.

## 무엇을 검증하나

| 단계 | 무엇을 |
|---|---|
| 설계 | xclbin `USER_METADATA` 를 `operation::read_descriptor()` 로 읽어 출력 |
| 인자 순서 | 바인딩 인덱스를 `descriptor::argument()` 로 조회 (손으로 세지 않는다) |
| 스트림 합성 | `program::wire()`. 타일 수·BD 수, 그리고 **헤더 4워드 해독** |
| 대조 | aiecc 스트림과 바이트 비교. **다르면 거기서 멈춘다** — 하드웨어가 할 말이 없다 |
| `amd::device` | 생성 |
| `load_op` | `register_xclbin` + `hw_context` |
| `make<...>` | operation 이 xclbin 에서 읽은 설계로 **다시** 배선하고 오프라인 스트림과 바이트 비교. 두 경로가 설계를 다르게 읽으면 여기서 걸린다 |
| `create_instance` | aiebu → ELF → module → kernel (`operation.cpp` 안에서) |
| `execute` | 완료 상태를 `std::error_code` 로 확인 |
| **수치** | CPU 레퍼런스와 비교. **0 인 원소 수도 센다** |
| 지연 | 반복 평균 (GEMM 은 GFLOP/s 도) |

0 인 원소가 전부면 "커널이 이 버퍼에 쓰지 않았다"고 명시한다 — 한 번 걸렸던 함정이라
다음에는 바로 보이게 해뒀다.

## 오퍼레이터별로 다른 점

| | rmsnorm | gemm |
|---|---|---|
| shape | 설계가 고정 (`elements`) | 스트림 파일 이름에서 (`.m128k3072n1536.`) |
| 케이스 수 | 설계당 1개 | 설계당 스트림 개수만큼 |
| 피연산자 | 아티팩트로 함께 옴 | **런타임 생성** — B 하나가 9 MB 라 커밋할 수 없다 |
| 정답 | `*.golden.bin` | 런타임 계산 |
| 허용 오차 | 상대 4% (IRON 이 쓰는 값) | 상대 2% |

GEMM 은 설계 하나에 shape 여럿을 **하나의 hw_context 에서** 돌린다. "코어가 자기 메모리에서
반복 횟수를 읽으니 xclbin 하나가 모든 프로젝션을 처리한다"는 것이 설계의 주장인데, shape 마다
어레이를 다시 열면 그 주장을 시험하지 않고 넘어가게 된다.

### GEMM 의 CPU 레퍼런스

피연산자는 splitmix64 로 만든다. `<random>` 의 분포는 비트 단위로 규정돼 있지 않아 표준
라이브러리마다 다른데, **디바이스가 받은 것과 다른 숫자로 계산한 정답은 정답이 아니다.**
레퍼런스는 float 이 아니라 저장된 bf16 을 읽어서 곱하므로, 피연산자 반올림은 양쪽에서
상쇄되고 남는 것은 산술뿐이다.

레이아웃은 에미터가 내는 BD 오프셋에서 확인한 것이다 (`b_col_maj=True` 설계):

| | 저장 형태 |
|---|---|
| A | `[m][k]` row-major |
| B | `[n][k]` row-major — 즉 `[k][n]` 의 전치 |
| C | `[m][n]` row-major |

허용 오차 2% 는 bf16 1 ULP (2^-8 = 0.39%) 의 대략 두 배다. 양쪽 다 같은 bf16 을 곱해 float 으로
누산하므로 남을 차이는 결과를 bf16 으로 되돌릴 때의 반올림뿐인데, 누산 순서가 다르면 값이
반올림 직전에 조금 움직여서 경계에 있던 값이 반대로 떨어질 수 있다. 레이아웃이 틀리면 이걸로
빠져나가지 못한다 — 마지막 자리가 아니라 자릿수가 어긋난다.

## 빌드

XRT 말고는 의존성이 없다. protobuf 도 onnx 도 필요 없다.

### Windows

Ryzen AI 소프트웨어 또는 NPU 드라이버 패키지의 XRT 가 필요하다.

```powershell
cmake -S . -B build -DXRT_DIR="C:/path/to/xrt"
cmake --build build --config Release
.\build\Release\nex-hwtest.exe ..\lnpu-artifacts\amd
```

`XILINX_XRT` 환경변수가 잡혀 있으면 `-DXRT_DIR` 은 생략해도 된다.

### Linux

```bash
cmake -S . -B build -DXRT_DIR=/opt/xilinx/xrt -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/nex-hwtest ../lnpu-artifacts/amd
```

인자는 `<아티팩트 디렉터리> [반복 횟수] [--dry]`. 디렉터리를 생략하면
`../lnpu-artifacts/amd`, 반복 횟수 기본값은 100 (`0` 이면 지연 측정 생략).

**Release 로 빌드할 것.** GEMM 레퍼런스가 최대 604 M MAC 이라 디버그 빌드로는 오래 걸린다.

### 건식 실행 (NPU 없이)

```
nex-hwtest ../lnpu-artifacts/amd 0 --dry
```

디바이스를 열지 않고 **스트림 생성과 aiecc 대조까지만** 한다. 개발 머신에서 7/7 통과한다:
rmsnorm 설계 3개와 gemm shape 4개.

## 아티팩트

`lnpu-artifacts/amd/` 하나만 본다. 예전에 `hwtest/artifacts/` 에 따로 두던 사본은 같은 설계가
다른 이름으로 중복돼 있던 것이라 없앴다.

| 파일 | 내용 |
|---|---|
| `<설계>.xclbin` | IRON/aiecc 가 구운 것. 설계 기술자가 `USER_METADATA` 섹션에 들어 있다 |
| `<설계>.aiecc.bin` | aiecc 가 만든 명령 스트림. 우리 것과 대조하는 용도 |
| `<설계>.<shape>.aiecc.bin` | shape 를 런타임에 정하는 설계의 스트림. 이름이 곧 케이스 정의다 |
| `<설계>.input.bin` | 입력 (bf16, little endian) |
| `<설계>.weight.bin` | 가중치 (weighted 케이스만) |
| `<설계>.golden.bin` | CPU 레퍼런스 결과 |

rmsnorm 기술자는 `elements` 를 함께 싣는다. IRON 의 rmsnorm 은 코어가 도는 횟수를 파이썬 레벨
상수로 굽기 때문에 (`design.py` 의 `for _ in range_(N_div_n)`) **한 설계는 한 길이만 돌린다.**
더 긴 입력을 주면 실패하지 않고 앞부분만 계산한 뒤 나머지를 그대로 두는데, 그건 틀린 답이
아니라 절반쯤 맞은 답처럼 보여서 더 나쁘다. 그래서 여기서 막는다.

## 결과를 어떻게 읽나

- **"aiecc 스트림과 바이트 일치"가 안 나오면** emitter 문제다. 하드웨어까지 갈 것도 없다.
- **"오프라인 스트림과 다름"이 나오면** 테스트가 읽은 기술자와 operation 이 읽은 기술자가
  다르다는 뜻이다. 둘 중 실제로 도는 것은 후자다.
- **적재에서 실패하면** XRT/드라이버 버전 문제일 가능성이 높다.
  `register_xclbin` + `hw_context` 2단계가 NPU shim 에서 필수인데, `load_xclbin` 은 동작하지 않는다.
- **실행은 되는데 수치가 틀리면** 서술자는 맞고 해석이 틀린 것이다.
  오차가 난 첫 원소 번호를 같이 찍으니, 그게 어느 코어의 slice 인지 보면 범위가 좁혀진다.
- **`state`** 는 XRT 의 `ert_cmd_state` 다. 4 = COMPLETED.

## 검증 결과 (NPU Strix, 2026-08-23)

rmsnorm 3/3 통과. 무가중치 두 케이스는 CPU 레퍼런스와 **비트 단위 일치**, 가중치 케이스는
bf16 곱셈 반올림으로 상대오차 0.0115 (허용치 4% 안). 186~208 µs.

GEMM 은 아직 실기기에서 돌리지 않았다.

### ⚠ `xrt::ext::bo` 를 가변 인자 커널 호출에 넘기지 말 것

넘기면 구버전 XRT 에서 `patch_value() only supports 64-bit values or less` 로 거부된다.
`xrt::run::set_arg` 의 일반 템플릿이 스칼라 경로를 타면서 `sizeof(xrt::ext::bo)` = 16 을
인자 값으로 패치하려 들기 때문이다. 최신 XRT 헤더에는 이를 잡는 오버로드가 있지만
Windows 쪽 드라이버 스택에는 없다. **평범한 `xrt::bo` 로 잡으면 양쪽 다 된다.**

- **커널 인자를 다 채우지 않는다.** xclbin 메타데이터는 `bo0..bo4` 5개를 선언하는데
  (aiecc 가 고정 개수로 패딩한다) 설계가 실제로 쓰는 것은 2~3개다. 쓰는 만큼만 넘겨도
  문제없다는 것이 확인되었다.
- `command.hpp` 가 `<xrt/xrt_hw_context.h>` 를 include 하지만 실제로 쓰지는 않는다.
  덕분에 emitter 는 XRT 없이도 빌드된다 (여기서는 어차피 링크하므로 무해).
- Windows 의 XRT 라이브러리 이름이 `xrt_coreutil` 이 아닐 수 있다. `find_library` 가 실패하면
  `-DXRT_DIR` 아래 `lib/` 를 확인하고 CMakeLists 의 `NAMES` 를 맞춰야 한다.
- aiebu 가 별도 라이브러리가 아니라 XRT 본체에 들어가 있는 배포도 있다. 그때는
  `find_library(AIEBU_LIB ...)` 를 지우고 `xrt_coreutil` 만 링크하면 된다.
