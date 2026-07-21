# XSPI2 PSRAM LCD 프레임 버퍼 변경 보고서

## 1. 문서 기준

- 비교 기준: 현재 Git `HEAD`의 원본과 작업 트리의 차이
- 대상 MCU/보드: MIMXRT700 계열 프로젝트
- 대상 메모리: AP Memory `APS256XXN-OB9-WA`
- 메모리 인터페이스: Xccela OPI DDR x8 (HyperRAM 아님)
- 사용 목적: LVGL/LCDIF 프레임 버퍼를 내부 RAM에서 XSPI2 PSRAM으로 이동
- 최종 확인 상태: PSRAM 읽기/쓰기와 `memset`/`memcpy` 통과, LCD 및 UI 정상 표시

## 2. 원본에서 발생한 문제

원본은 LCD 프레임 버퍼를 내부 RAM 주소 `0x20200000`, `0x20280000`에서 사용했다. 이를 XSPI2 PSRAM 주소로 단순 변경했을 때 다음 문제가 순차적으로 확인됐다.

1. APS256XXN과 맞지 않는 명령/LUT 설정으로 PSRAM 테스트 실패
2. 32비트 쓰기는 성공하지만 8비트, 16비트, `memset` 쓰기는 실패
3. 초기 PSRAM 테스트는 통과해도 전원 절감 코드 실행 후 접근 불가
4. CPU 접근은 가능해도 LCDIF가 PSRAM 프레임을 안정적으로 가져오지 못함
5. LCDIF/DSI 클럭을 프레임 단위로 정지·재시작할 때 화면이 표시되지 않음

대표 진단 결과는 다음과 같았다.

```text
write8   FAIL
write16  FAIL
write32  PASS
write64  PASS
memset8  FAIL
```

최종 DQS/DM 출력 설정을 수정한 후 다음과 같이 바뀌었다.

```text
write8       PASS
write16      PASS
write32      PASS
write64      PASS
memset8      PASS
memset-frame PASS (447360 bytes)
memcpy-frame PASS (447360 bytes)
```

## 3. 핵심 원인과 해결

### 3.1 APS256XXN 전용 XSPI2 초기화

파일: `rt700_dual_core_low_power_thermometer/armgcc/examples/_boards/mimxrt700evk/board.c`

원본의 일반 PSRAM 초기화와 별도로 `XSPI2_PSRAM_Init()`을 추가했다.

주요 설정은 다음과 같다.

| 항목 | 적용값 | 이유 |
|---|---:|---|
| Linear burst read | `0x20` | APS256XXN Xccela read 명령 |
| Linear burst write | `0xA0` | APS256XXN Xccela write 명령 |
| 주소 | 32-bit RADDR | 전체 바이트 주소 전달 |
| `columnAddrWidth` | `0` | 별도 CADDR 분리 방지 |
| Page size | 2048 bytes | APS256XXN 페이지 크기 |
| AHB split | 64 bytes | 검증된 정상 설정 |
| XSPI2 clock | FRO0 192 MHz / 2 = 96 MHz | 전원 초기화 후에도 유지되는 클럭 사용 |
| Sample clock | External DQS | OPI DDR 데이터 샘플링 |

또한 전원 인가 후 150 us 대기, Global Reset 전송, 후속 안정화 지연 및 AHB/TX/RX 버퍼 초기화를 추가했다.

### 3.2 8/16비트 및 `memset` 실패 해결

가장 중요한 수정은 다음 코드다.

```c
XSPI_EnableModule(XSPI2, false);
XSPI2->MCR |= XSPI_MCR_DQS_OUT_EN_MASK;
XSPI_EnableModule(XSPI2, true);
```

SDK의 `XSPI_SetDeviceConfig()`는 Standard Extended SPI 장치에 대해 `MCR[DQS_OUT_EN]`을 해제한다. 그러나 APS256XXN은 쓰기 중 DQS 핀을 active-high DM(Data Mask)으로 사용한다.

이 출력이 비활성화되면 32/64비트 전체 워드 쓰기는 우연히 동작할 수 있지만, 바이트 스트로브가 필요한 8비트·16비트 쓰기와 `memset`은 적용되지 않는다. 장치 설정 후 `DQS_OUT_EN`을 다시 활성화해 부분 쓰기를 정상화했다.

### 3.3 SDK 페이지 크기 계산 오류 수정

파일: `rt700_dual_core_low_power_thermometer/armgcc/drivers/xspi/fsl_xspi.c`

`XSPI_GetPPWBFromPageSize()`의 2048/4096바이트 `case`에 `break`가 없어 다음 case/default로 fall-through 하던 부분을 수정했다.

```c
case 2048UL:
    ppwbValue = 11U;
    break;
case 4096UL:
    ppwbValue = 12U;
    break;
```

### 3.4 전원 관리 후 XSPI2 접근 불가 해결

파일: `rt700_dual_core_low_power_thermometer/armgcc/source/src/power_management.c`

원본의 미사용 클럭/모듈 차단 코드가 부팅 후 XSPI2와 Media AXI 경로를 끄고 있었다. 프레임 버퍼 사용 중에는 다음 리소스를 유지하도록 변경했다.

- `kCLOCK_Xspi2`
- `kCLOCK_MediaAccessRamArbiter1`
- `kCLOCK_Axi0`
- `kCLOCK_Syscon4`
- `kPDRUNCFG_APD_XSPI2`
- `kPDRUNCFG_PPD_XSPI2`

PLL 관련 power-down도 현재 주석 처리되어 있다. XSPI2 자체는 FRO0를 사용하지만 LCD/DSI 및 다른 주변장치 클럭 영향 여부를 고려한 변경이다.

### 3.5 Sleep/Deep Sleep 제약 추가

파일: `rt700_dual_core_low_power_thermometer/armgcc/examples/_boards/mimxrt700evk/lvgl_examples/app.h`

저전력 진입 시 XSPI2 SRAM 상태가 소실되지 않도록 제약을 추가했다.

- Sleep: `PM_RESC_SRAM_XSPI2_ACTIVE`
- Deep sleep: `PM_RESC_SRAM_XSPI2_RETENTION`
- Sleep constraint 개수: 27 → 28
- Deep sleep constraint 개수: 11 → 12

### 3.6 LCDIF의 PSRAM 접근 경로 설정

파일: `rt700_dual_core_low_power_thermometer/armgcc/examples/_boards/mimxrt700evk/display_support.c`

`BOARD_ConfigureCo5300PsramLcdifMaster()`를 추가했다.

- LCDIF master ID `0x1D` 전용 XSPI AHB buffer: 2 KB
- 모든 master가 사용할 shared buffer: 2 KB
- NIC Media1 ASIB[7] read QoS: 1

CPU뿐 아니라 LCDIF 같은 비CPU AHB master도 PSRAM을 연속 읽을 수 있도록 한 설정이다.

### 3.7 프레임 전송 직전 AHB 데이터 정합성 보장

파일: `rt700_dual_core_low_power_thermometer/armgcc/examples/lvgl_examples/lvgl_sdk/lvgl_support/rt700/lvgl_support.c`

실제 LCD 프레임 버퍼 전환 전 다음 처리를 추가했다.

```c
__DSB();
XSPI_ClearAhbBuffer(XSPI2);
```

CPU의 PSRAM 쓰기가 완료된 뒤 LCDIF가 최신 프레임 데이터를 읽도록 순서를 보장한다.

또한 화면 사용 중에는 DSI/LCDIF 클럭을 유지하도록 buffer-done callback의 `BOARD_StopMIPIClock()`과 첫 flush의 `BOARD_StartMIPIClock()` 호출을 제거했다.

## 4. 프레임 버퍼 변경

파일: `rt700_dual_core_low_power_thermometer/armgcc/examples/_boards/mimxrt700evk/display_support.h`

| 구분 | Buffer 0 | Buffer 1 |
|---|---:|---:|
| 원본 내부 RAM | `0x20200000` | `0x20280000` |
| 변경 XSPI2 PSRAM | `0x60000000` | `0x60200000` |

프레임 크기는 447,360 bytes이며 두 버퍼 사이에는 충분한 주소 간격이 있다.

## 5. 초기화 순서 변경

파일: `rt700_dual_core_low_power_thermometer/armgcc/examples/_boards/mimxrt700evk/lvgl_examples/hardware_init.c`

초기화 순서에 다음을 추가했다.

1. `BOARD_InitPsRamPins_Xspi2()`
2. `BOARD_BootClockRUN()`
3. `XSPI2_PSRAM_Init()`
4. 이후 LVGL/LCD 초기화

`XSPI2_PSRAM_MemoryTest()` 호출과 PASS/FAIL 로그는 현재 `#if 0`으로 비활성화되어 있다. 필요 시 제조 검사나 디버그 빌드에서만 활성화할 수 있다.

## 6. 핀 및 보드 종속 변경

다음 변경은 PSRAM 프로토콜 수정과 별개로 현재 보드 배선에 종속된다.

### MIPI 제어 핀

파일: `board.h`, `lvgl_examples/pin_mux.c`

- MIPI reset: GPIO4_12 → GPIO7_22
- MIPI power: GPIO4_13 → GPIO7_23
- DCDC 1.8 V enable 정의: GPIO1_15
- DCDC 3.3 V enable 정의: GPIO1_16

DCDC GPIO 초기화 호출은 현재 주석 처리되어 있으므로 실제 전원 회로에서 필요하면 별도로 활성화해야 한다.

### XSPI2 핀

`BOARD_InitPsRamPins_Xspi2()`가 초기화 과정에 추가됐다. 다른 UI 프로젝트에도 동일 PSRAM을 사용하려면 해당 프로젝트의 pin mux에 XSPI2 CLK, CS, DQS, DATA[7:0] 설정이 모두 존재해야 한다.

## 7. 현재 검증 결과

- XSPI2 PSRAM pattern/invert memory test: PASS
- 8-bit write: PASS
- 16-bit write: PASS
- 32-bit write: PASS
- 64-bit write: PASS
- `memset` word verification: PASS
- 전체 프레임 `memset` 447,360 bytes: PASS
- 전체 프레임 `memcpy` 447,360 bytes: PASS
- 내부 RAM 프레임 버퍼 LCD 표시: PASS
- PSRAM 프레임 버퍼 LCD 표시: PASS
- LVGL UI 표시: PASS

## 8. 다른 UI 프로젝트에 이식할 필수 항목

다른 프로젝트에서 UI 코드만 변경하는 것으로는 충분하지 않다. 아래 항목을 함께 이식해야 한다.

1. XSPI2 pin mux 및 `BOARD_InitPsRamPins_Xspi2()` 호출
2. APS256XXN 전용 LUT와 `XSPI2_PSRAM_Init()`
3. `XSPI_SetDeviceConfig()` 후 `DQS_OUT_EN` 재활성화
4. XSPI2/Media AXI/SYSCON4 클럭 및 power domain 유지
5. Sleep/Deep sleep의 XSPI2 active/retention 제약
6. LCDIF master용 XSPI AHB buffer 및 QoS 설정
7. 프레임 전송 전 `__DSB()`와 `XSPI_ClearAhbBuffer()`
8. 프레임 버퍼 주소를 `0x60000000`, `0x60200000`으로 지정
9. 화면 사용 중 DSI/LCDIF 클럭을 임의로 끄지 않도록 확인
10. 표준 `memset`/`memcpy` 전체 프레임 검증 후 LVGL 연결

권장 검증 순서는 `write8 → write16 → write32 → write64 → memset → memcpy → LCD 단색 프레임 → LVGL UI`이다.

## 9. 재검토가 필요한 현재 변경

다음은 PSRAM/LCD 정상화의 필수 변경으로 보기 어렵거나 제품 동작에 별도 영향을 줄 수 있다.

### LVGL 애플리케이션 대기 코드 비활성화

파일: `lvgl_guider.c`

현재 다음 코드가 주석 처리되어 있다.

```c
xSemaphoreTake(xMUBinarySemaphore, portMAX_DELAY);
UpdateDisplayedTemperature(*(int32_t*)&result);
APP_SetWakeupConfig();
xSemaphoreTake(xRTCBinarySemaphore, portMAX_DELAY);
```

이 변경은 UI 루프가 멈추는 현상을 피하는 데 사용됐지만, 온도 갱신, wakeup 설정, RTC 동기화 동작을 제거한다. 시간/온도 표시와 저전력 동작을 최종 확정할 때 원복 또는 타임아웃 기반 처리로 다시 설계해야 한다.

### PLL 유지

`power_management.c`에서 MAIN/AUDIO PLL power-down이 주석 처리되어 있다. 최종 전력 최적화 단계에서 실제 LCD/DSI/오디오 클럭 의존성을 측정한 뒤 필요한 PLL만 유지하는 것이 좋다.

### 테스트 코드 정리

- `board.c`의 `#if 1`은 의미 있는 feature macro로 교체 권장
- `hardware_init.c`의 `#if 0` 테스트 블록은 debug option으로 교체 권장
- `XSPI_PSRAM_PATTERN_CHUNK_WORDS`, `XSPI_PSRAM_STRESS_BYTES`, `XSPI_PSRAM_TEST_SKIPPED`처럼 현재 사용하지 않는 define은 제거 가능

## 10. 결론

이번 문제의 핵심은 단순한 프레임 버퍼 주소 변경이 아니었다. APS256XXN의 Xccela 명령 구성, DQS의 write DM 기능, 전원 관리 이후 XSPI2/Media AXI 유지, 그리고 LCDIF 비CPU master 접근을 모두 맞춰야 했다.

특히 `DQS_OUT_EN` 수정 전에는 32비트 테스트만 통과해 메모리가 정상처럼 보였지만 `memset`과 LVGL의 8/16비트 쓰기가 실제로 적용되지 않았다. 따라서 다른 프로젝트에서도 32비트 pattern test만으로 판단하지 말고 바이트·하프워드·전체 프레임 `memset`/`memcpy`를 반드시 함께 검증해야 한다.
