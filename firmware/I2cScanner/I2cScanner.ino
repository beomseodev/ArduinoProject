/*
 * I2cScanner.ino
 * I2C 버스에 연결된 장치 주소를 스캔 — PCA9685 등이 잡히는지 확인할 때 사용
 *
 * PCA9685: 기본 7비트 주소는 보통 0x40 (모듈의 A0~A5 점퍼에 따라 0x41~0x7F 등으로 바뀔 수 있음)
 * 배선: SDA/SCL/GND 공통, 전원은 모듈 규격에 맞게 (3.3V 전용 버스면 5V 직결 금지)
 *
 * 시리얼 모니터: 9600 baud, 개행(LF) 권장
 *
 * 수정일: 2026-04-15
 * 변경: 최초 작성 — I2C 스캐너로 버스 응답 확인
 */

#include <Wire.h>  // 아두이노 기본 I2C(와이어) 라이브러리 — 별도 설치 없음

void setup() {
  Wire.begin();  // I2C 마스터로 시작 (Mega: 기본 SDA 20, SCL 21)
  Serial.begin(9600);
  Serial.println(F("I2C Scanner Start"));
}

void loop() {
  byte error;
  byte address;
  int count = 0;

  // 7비트 I2C 주소는 1 ~ 127 범위를 조사
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    // 0: ACK(장치 있음), 1: 데이터 과다, 2: 주소 NACK, 3: 데이터 NACK, 4: 기타
    if (error == 0) {
      Serial.print(F("I2C device found at 0x"));
      if (address < 16) {
        Serial.print(F("0"));
      }
      Serial.println(address, HEX);
      count++;
    }
  }

  if (count == 0) {
    Serial.println(F("No I2C devices found"));
  } else {
    Serial.print(F("Total: "));
    Serial.println(count);
  }

  delay(3000);
}
