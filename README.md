# ArduinoProject

SunFounder **Mercury Board**(Arduino Mega 2560 호환)를 기반으로 한 미니 프로젝트 모음입니다.

---

## PC·보드 연결 확인

업로드 후 보드 **내장 LED**(일반적으로 D13)가 1초마다 깜빡이면 USB·드라이버·포트·보드 선택이 대체로 맞습니다.

- 스케치: [`firmware/MercuryConnectionTest/MercuryConnectionTest.ino`](firmware/MercuryConnectionTest/MercuryConnectionTest.ino)
- Arduino IDE: 보드 **Arduino Mega or Mega 2560**, 도구 → 포트에서 Mercury에 해당하는 **COM** 선택

---

## I2C 스캐너로 PCA9685 연결 확인

PCA9685는 I²C 장치이므로, **SDA/SCL·GND**가 보드와 맞게 연결되어 있으면 스캐너 출력에 **0x40**(기본 주소) 또는 점퍼 설정에 따른 다른 주소가 나타납니다. 아무 주소도 없으면 배선·전원·풀업·버스 단락 여부를 먼저 확인합니다.

- 스케치: [`firmware/I2cScanner/I2cScanner.ino`](firmware/I2cScanner/I2cScanner.ino)
- 라이브러리: `Wire` (Arduino IDE 기본)
- 시리얼 모니터: **9600** baud

---

## SG90 + PCA9685 연결 테스트

PCA9685 **PWM 0번**에 SG90 신호선을 연결한 뒤, 스케치는 **중앙 → 한쪽 → 반대쪽**으로 펄스 폭을 바꿉니다. 서보 축이 따라 움직이면 배선·전원·I²C가 대체로 정상입니다. 전류 부족 시 리셋·떨림이 나올 수 있으므로 **서보 5V는 외부 공급**과 **공통 GND**를 권장합니다.

- 스케치: [`firmware/Sg90Pca9685Test/Sg90Pca9685Test.ino`](firmware/Sg90Pca9685Test/Sg90Pca9685Test.ino)
- 라이브러리: Arduino 라이브러리 매니저에서 **Adafruit PWM Servo Driver** (의존성 자동 설치)
- PCA9685 주소가 0x40이 아니면 스케치의 `Adafruit_PWMServoDriver(0x40)` 인자를 I2C 스캐너 값에 맞게 수정

---

## HW-040 로터리 인코더 연결 테스트

인코더를 돌리면 시리얼에 **Clockwise / CounterClockwise**, 축을 누르면 **Button pressed**가 출력되면 배선·풀업이 대체로 맞습니다. 이 스케치는 **D29(CLK), D28(DT), D27(SW)** 기준이며, 쉴드 핀 위치에 맞게 스케치 상단 상수만 바꾸면 됩니다.

- 스케치: [`firmware/Hw040EncoderTest/Hw040EncoderTest.ino`](firmware/Hw040EncoderTest/Hw040EncoderTest.ino)
- 라이브러리: 없음 (`Wire` 불필요)
- 시리얼 모니터: **9600** baud

---

## HW-040 로터리 인코더로 SG90 서보 제어

**HW-040**(로터리 인코더 모듈)의 회전·버튼 입력을 읽어 **SG90** 서보 모터의 각도(또는 PWM 출력)를 조절한 프로젝트입니다.

### 사용 부품

| 부품 | 역할(개요) |
|------|------------|
| [SunFounder Mercury Board](http://wiki.sunfounder.cc/index.php?title=SunFounder_Mercury_Board) | 메인 MCU. Mega 2560 호환, Arduino IDE에서 보드 타입을 Mega로 선택해 사용 |
| **Arduino Mega Sensor Shield V2.0** | Mercury 위에 적층해 센서·모듈을 3핀(GND/5V/신호) 배선으로 쉽게 연결 |
| **PCA9685** | I²C 16채널 PWM 드라이버. MCU PWM 핀 부담을 줄이고 서보 구동에 자주 사용 |
| **HW-040** | 증분식 로터리 인코더(CLK, DT, SW 등). 회전 방향·스텝으로 값 변경, 축 누름은 스위치 |
| **SG90** | 마이크로 서보. PCA9685 채널 또는 보드 PWM에 연결(실제 배선은 스케치·회로에 맞게) |

### 동작 아이디어(참고)

- 인코더를 돌릴 때마다 **목표 각도**(또는 PWM 펄스 폭)를 일정 스텝만큼 증감시키고, 그 값을 PCA9685(또는 `Servo` 라이브러리)로 SG90에 반영합니다.
- HW-040은 고속 회전 시 펄스를 놓치지 않도록 **인터럽트**(가능한 핀) 사용을 권장합니다.
- 전원: 서보는 순간 전류가 커질 수 있으므로 **외부 5V 공급 + 공통 GND** 등 전원 설계를 스케치 주석이나 별도 문서에 적어 두면 재현에 도움이 됩니다.

### 필요 라이브러리(예시)

- PCA9685: `Adafruit PWM Servo Driver` 등 I²C용 서보 드라이버 라이브러리
- 인코더: 직접 핀 읽기 또는 `Encoder` 등(보드·핀에 맞게 선택)

### 통합 스케치

인코더 한 스텝마다 펄스 폭을 **±100µs** 조정하고(범위 약 **500~2300µs**), 축 버튼으로 **1400µs** 부근(중앙)으로 되돌립니다. 회전 방향이 기대와 반대면 스케치 안에서 **증감 블록을 서로 바꾸거나** CLK/DT 배선을 맞바꿉니다.

- 스케치: [`firmware/Hw040Sg90Servo/Hw040Sg90Servo.ino`](firmware/Hw040Sg90Servo/Hw040Sg90Servo.ino)
- 시리얼 모니터: **9600** baud

---

## 코드는 어떻게 넣으면 좋을지

이 저장소는 `firmware/스케치이름/스케치이름.ino` 규칙을 따릅니다. 스케치가 커지면 아래처럼 나누면 수정 범위를 줄일 수 있습니다.

1. **폴더 이름 = Arduino IDE 스케치 규칙** — 폴더명과 `.ino` 파일명을 동일하게 유지  
2. **선택적 분리** — `config.h`(핀·I²C 주소·펄스 한계), `encoder_*.cpp`(인터럽트 기반 읽기), `servo_pca9685.*`(출력만) 등  
3. **상단 주석** — 배선·날짜·변경 요약을 스케치 머리에 두면 README와 중복을 줄일 수 있음
