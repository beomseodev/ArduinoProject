/*
 * MercuryConnectionTest.ino
 * SunFounder Mercury Board(Arduino Mega 2560 호환) — PC 연결·업로드 확인용
 *
 * 동작: 보드에 내장된 LED(보통 D13)가 1초 간격으로 깜빡이면
 *       시리얼 업로드와 기본 실행이 정상인 것으로 보면 됩니다.
 *
 * Arduino IDE 설정 예:
 *   보드: "Arduino AVR Boards" → Arduino Mega or Mega 2560
 *   포트: 장치 관리자/IDE에서 Mercury에 해당하는 COM 포트
 *
 * 수정 이력:
 *   2026-04-15 — 최초 작성(LED 깜빡임 연결 확인)
 *   2026-04-15 — IDE(clang) 분석용 `#include <Arduino.h>` 추가, compile_commands 상대 경로 정합
 */

#include <Arduino.h>  // LED_BUILTIN, pinMode, delay 등 — 에디터 정적 분석용

void setup() {
  // 내장 LED 핀을 출력으로 설정 (Mega 계열에서는 보통 13번 = LED_BUILTIN)
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // LED 켜기
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);     // LED 끄기
  delay(1000);
}
