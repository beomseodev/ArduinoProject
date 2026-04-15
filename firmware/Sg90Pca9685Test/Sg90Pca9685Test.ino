/*
 * Sg90Pca9685Test.ino
 * PCA9685 채널 0에 연결한 SG90이 움직이는지 확인하는 테스트 스케치
 *
 * 배선(요약):
 *   PCA9685 SDA/SCL → 보드 I2C (Mega: SDA 20, SCL 21)
 *   PCA9685 V+ / GND → 서보용 5V·GND (가능하면 USB 5V만으로 버티지 말고 외부 5V 권장)
 *   SG90 신호선(주황) → PCA9685 PWM 0번 출력
 *   SG90 빨강/갈색 → 서보 전원 5V/GND (보드 GND와 공통)
 *
 * 라이브러리 매니저: "Adafruit PWM Servo Driver" 설치 (의존성 Adafruit BusIO 등 함께 설치됨)
 *
 * 수정일: 2026-04-15
 * 변경: 최초 작성 — SG90 중앙·좌·우 펄스 테스트 (50Hz, 채널 0)
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// I2C 주소 0x40 (모듈 점퍼가 다르면 생성자 인자만 바꿔 주면 됨)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// PCA9685 PWM 출력 번호 (0~15). SG90 신호선이 꽂은 채널과 맞출 것
const uint8_t kServoChannel = 0;

void setup() {
  Serial.begin(9600);

  pwm.begin();
  // 오실레이터 주파수 보정(보드·칩에 따라 미세 조정 가능, 기본 예시와 동일)
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);  // 서보용 일반값 약 50Hz
  delay(10);

  Serial.println(F("SG90 / PCA9685 servo test start"));
}

void loop() {
  // 펄스 폭(마이크로초): 기종·개체마다 최소/최대가 조금씩 다를 수 있음
  pwm.writeMicroseconds(kServoChannel, 1400);  // 대략 중앙
  delay(1000);

  pwm.writeMicroseconds(kServoChannel, 500);  // 한쪽 끝
  delay(1000);

  pwm.writeMicroseconds(kServoChannel, 2300);  // 반대 끝
  delay(1000);
}
