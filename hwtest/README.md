# RMSNorm 하드웨어 테스트

NPU가 달린 장비에서 **이 저장소의 백엔드가 실제로 도는지, 그리고 결과가 맞는지** 확인한다.

XRT 를 직접 부르지 않는다. 스트림은 `amd::programs::rmsnorm` 이 만들고, 어레이는
`amd::device` 가 열고, 실행은 `amd::operation` 을 거친다 — 런타임을 이루는 바로 그 객체들이다.
이걸 우회하면 정확히 놓치기 쉬운 것을 놓친다: `device` 에는 생성자 정의조차 없었는데,
아무도 만들어 본 적이 없어서 아무도 몰랐다.

개발 머신에는 NPU가 없어서 지금까지는 aiecc 가 만든 스트림과 **바이트가 같다**는 것만
확인했다. 바이트가 같다는 것과 하드웨어가 받아들인다는 것은 다른 얘기고, 이게 그 차이를
메우는 부분이다.

## 무엇을 검증하나

| 단계 | 무엇을 |
|---|---|
| 아티팩트 | 파일 크기까지 찍는다 |
| 설계 | xclbin `USER_METADATA` 를 `amd::descriptor` 로 읽어 커널 시그니처와 함께 출력 |
| 크기 | 입력 원소 수가 설계가 구워진 길이와 같은지. **다르면 거기서 멈춘다** |
| 인자 순서 | 바인딩 인덱스를 `descriptor::argument()` 로 조회 (손으로 세지 않는다) |
| 스트림 합성 | `programs::rmsnorm::wire()`. 코어 수·slice·BD 수, 그리고 **헤더 4워드 해독** |
| 대조 | aiecc 스트림과 바이트 비교. **다르면 거기서 멈춘다** — 하드웨어가 할 말이 없다 |
| `amd::device` | 생성 |
| `load_op` | `register_xclbin` + `hw_context` |
| `create_instance` | aiebu → ELF → module → kernel (`operation.cpp` 안에서) |
| 버퍼 | 크기와 디바이스 주소 |
| `execute` | 완료 상태를 `std::error_code` 로 확인 |
| **수치** | CPU 레퍼런스와 비교 (상대 4%, 절대 1e-6). **0 인 원소 수도 센다** |
| 지연 | 반복 평균 |

0 인 원소가 전부면 "커널이 이 버퍼에 쓰지 않았다"고 명시한다 — 한 번 걸렸던 함정이라
다음에는 바로 보이게 해뒀다.

## 빌드

XRT 말고는 의존성이 없다. protobuf 도 onnx 도 필요 없다.

### Windows

Ryzen AI 소프트웨어 또는 NPU 드라이버 패키지의 XRT 가 필요하다.

```powershell
cmake -S . -B build -DXRT_DIR="C:/path/to/xrt"
cmake --build build --config Release
.\build\Release\rmsnorm-hwtest.exe artifacts
```

`XILINX_XRT` 환경변수가 잡혀 있으면 `-DXRT_DIR` 은 생략해도 된다.

### Linux

```bash
cmake -S . -B build -DXRT_DIR=/opt/xilinx/xrt
cmake --build build -j
./build/rmsnorm-hwtest artifacts
```

두 번째 인자는 반복 횟수 (기본 100, `0` 이면 지연 측정 생략).

### 건식 실행 (NPU 없이)

```
rmsnorm-hwtest artifacts 0 --dry
```

디바이스를 열지 않고 **스트림 생성과 aiecc 대조까지만** 한다. 아티팩트가 성했는지,
emitter 가 기대대로 도는지를 하드웨어 없이 먼저 볼 수 있다. 개발 머신에서 확인한 결과:

```
=== rmsnorm_sz2048_c1_ch1_w1
  설계: columns=1 channels=1 weighted=yes, 원소 2048개
  명령 스트림: 105 워드 (420 바이트)
  aiecc 스트림과 바이트 일치
...
3개 중 3개 통과
```

## 아티팩트

`artifacts/` 에 케이스마다 다음이 들어 있다.

| 파일 | 내용 |
|---|---|
| `*.xclbin` | IRON/aiecc 가 구운 것. 설계 기술자가 `USER_METADATA` 섹션에 들어 있다 |
| `*.input.bin` | 입력 (bf16, little endian) |
| `*.weight.bin` | 가중치 (weighted 케이스만) |
| `*.golden.bin` | CPU 레퍼런스 결과 |
| `*.aiecc.bin` | aiecc 가 만든 명령 스트림. 우리 것과 대조하는 용도 |
| `*.design.json` | xclbin 에서 그대로 뽑아 쓴 것 (사람이 읽으라고 옆에도 둠). 읽는 쪽은 xclbin 만 본다 |

케이스 세 개 모두 **배치까지 유도되는 조합**이다 — 즉 설계 기술자의 숫자만으로
스트림 전체가 나온다.

설계 기술자는 `elements` 를 함께 싣는다. IRON 의 rmsnorm 은 코어가 도는 횟수를
파이썬 레벨 상수로 굽기 때문에 (`design.py` 의 `for _ in range_(N_div_n)`) **한 설계는
한 길이만 돌린다.** 더 긴 입력을 주면 실패하지 않고 앞부분만 계산한 뒤 나머지를 그대로
두는데, 그건 틀린 답이 아니라 절반쯤 맞은 답처럼 보여서 더 나쁘다. 그래서 여기서 막는다.

| 케이스 | 정규화 형태 | 컬럼 | 가중치 |
|---|---|---:|---|
| `rmsnorm_sz2048_c1_ch1_w1` | 2048짜리 벡터 1개 | 1 | 있음 |
| `rmsnorm_sz2048_c8_ch1_w0` | 256짜리 벡터 8개 | 8 | 없음 |
| `rmsnorm_sz4096_c4_ch1_w0` | 1024짜리 벡터 4개 | 4 | 없음 |

## 결과를 어떻게 읽나

- **"aiecc 스트림과 바이트 일치"가 안 나오면** emitter 문제다. 하드웨어까지 갈 것도 없다.
- **적재에서 실패하면** XRT/드라이버 버전 문제일 가능성이 높다.
  `register_xclbin` + `hw_context` 2단계가 NPU shim 에서 필수인데, `load_xclbin` 은 동작하지 않는다.
- **실행은 되는데 수치가 틀리면** 서술자는 맞고 해석이 틀린 것이다.
  오차가 난 첫 원소 번호를 같이 찍으니, 그게 어느 코어의 slice 인지 보면 범위가 좁혀진다.
- **`state`** 는 XRT 의 `ert_cmd_state` 다. 4 = COMPLETED.

## 검증 결과 (NPU Strix, 2026-08-22)

3/3 통과. 무가중치 두 케이스는 CPU 레퍼런스와 **비트 단위 일치**, 가중치 케이스는
bf16 곱셈 반올림으로 상대오차 0.0115 (허용치 4% 안). 130~200 µs.

`amd::device` → `load_op` → `op()` → `create_instance` → `execute` 경로도 같은 결과다.

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
