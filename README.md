# ArduinoProject

SunFounder **Mercury Board**(Arduino Mega 2560 호환)를 기반으로 한 미니 프로젝트 모음입니다.

---

## PC·보드 연결 확인

업로드 후 보드 **내장 LED**(일반적으로 D13)가 1초마다 깜빡이면 USB·드라이버·포트·보드 선택이 대체로 맞습니다.

- 스케치: [`firmware/MercuryConnectionTest/MercuryConnectionTest.ino`](firmware/MercuryConnectionTest/MercuryConnectionTest.ino)
- Arduino IDE: 보드 **Arduino Mega or Mega 2560**, 도구 → 포트에서 Mercury에 해당하는 **COM** 선택

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

---

## 코드는 어떻게 넣으면 좋을지

아래는 나중에 스케치를 Git에 올릴 때 **읽기·수정·재현**이 쉬운 구성 예시입니다. 지금 저장소에 `.ino`가 없어도, 코드를 가져올 때 이 구조를 맞추면 README와 잘 맞습니다.

1. **폴더 이름 = Arduino IDE 스케치 규칙**  
   `firmware/Hw040Sg90Servo/Hw040Sg90Servo.ino` 처럼, 폴더명과 `.ino` 파일명을 동일하게 맞춥니다.

2. **한 파일에만 몰지 않기(선택)**  
   - `config.h` — I²C 주소, PCA9685 채널 번호, 인코더 핀, 각도 최소/최대, 스텝 크기  
   - `encoder_hw040.*` — HW-040 읽기·디바운스·인터럽트  
   - `servo_pca9685.*` — PCA9685로 각도 설정  
   - `Hw040Sg90Servo.ino` — `setup` / `loop`만 두어 **전체 흐름이 한눈에** 보이게  

3. **상단 주석 블록**  
   - 날짜, 변경 요약(사용자 규칙대로)  
   - 배선표: 보드 핀 ↔ 모듈 핀, PCA9685 채널 ↔ SG90 신호선  

4. **README와 연결**  
   - 이 파일의 「사용 부품」은 하드웨어 개요용  
   - 핀맵·빌드 방법은 스케치 상단 또는 `firmware/README.md`(원할 때만 추가)에 두면 중복을 줄일 수 있습니다.

원하시면 실제 `.ino` 내용을 붙여 주시면, 위 구조에 맞게 파일 분리·주석(한글)까지 맞춰 드리겠습니다.
