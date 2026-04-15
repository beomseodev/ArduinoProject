/*
 * Hw040Sg90Servo.ino
 * HW-040 로터리 인코더로 SG90 펄스 폭(마이크로초)을 바꾸고, PCA9685 채널 0으로 출력
 *
 * 배선 요약:
 *   HW-040: CLK→D29, DT→D28, SW→D27 (+5V/GND)
 *   PCA9685: I2C(SDA/SCL), 서보용 5V·GND, SG90 신호→PWM0
 *
 * 라이브러리: Adafruit PWM Servo Driver (+ 의존성)
 *
 * 수정일: 2026-04-15
 * 변경: 최초 작성 — 인코더 스텝으로 펄스 폭 조절, 버튼으로 중앙(1400us) 리셋
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// --- 하드웨어·튜닝 상수 (필요 시 여기만 수정) ---
const uint8_t kPca9685Address = 0x40;  // I2C 주소 (스캐너 값에 맞출 것)
const uint8_t kServoChannel = 0;     // SG90 신호가 연결된 PCA9685 PWM 번호

const int CLK_PIN = 29;  // HW-040 CLK
const int DT_PIN = 28;   // HW-040 DT
const int SW_PIN = 27;   // HW-040 SW

// SG90 계열에서 흔한 펄스 범위(개체마다 다를 수 있음)
const int kPulseMin = 500;
const int kPulseMax = 2300;
const int kPulseCenter = 1400;
const int kPulseStep = 100;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(kPca9685Address);

int lastClkState;
int angleStatus = kPulseCenter;

void setup() {
  Serial.begin(9600);

  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DT_PIN, INPUT_PULLUP);
  pinMode(SW_PIN, INPUT_PULLUP);
  lastClkState = digitalRead(CLK_PIN);

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);  // Sg90Pca9685Test와 동일 보정(필요 시 조정)
  pwm.setPWMFreq(50);
  delay(10);
  pwm.writeMicroseconds(kServoChannel, angleStatus);

  Serial.println(F("HW-040 + SG90 test start"));
}

void loop() {
  // 1) 회전: CLK 에지에서 DT와 비교해 방향 판별 (Hw040EncoderTest와 동일 패턴)
  int currentClkState = digitalRead(CLK_PIN);

  if (currentClkState != lastClkState) {
    if (digitalRead(DT_PIN) != currentClkState) {
      if (angleStatus > kPulseMin) {
        angleStatus -= kPulseStep;
        pwm.writeMicroseconds(kServoChannel, angleStatus);
      }
      Serial.print(F("Clockwise | angleStatus = "));
      Serial.println(angleStatus);
    } else {
      if (angleStatus < kPulseMax) {
        angleStatus += kPulseStep;
        pwm.writeMicroseconds(kServoChannel, angleStatus);
      }
      Serial.print(F("CounterClockwise | angleStatus = "));
      Serial.println(angleStatus);
    }
  }

  lastClkState = currentClkState;

  // 2) 버튼: 중앙 펄스로 리셋
  if (digitalRead(SW_PIN) == LOW) {
    angleStatus = kPulseCenter;
    pwm.writeMicroseconds(kServoChannel, angleStatus);
    Serial.println(F("Button pressed (center)"));
    delay(200);  // 간단한 디바운스
  }
}
