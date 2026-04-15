/*
 * Hw040EncoderTest.ino
 * HW-040 로터리 인코더(CLK, DT, SW) 연결 후 시리얼로 회전 방향·버튼 입력 확인
 *
 * 핀 배선(이 스케치 기준, Mega + Sensor Shield 등에 맞게 바꿀 수 있음):
 *   CLK → D29, DT → D28, SW → D27, GND·VCC(보통 5V)는 모듈 규격에 맞게
 *
 * 동작: CLK 에지에서 DT와의 관계로 시계/반시계를 출력, SW는 누르면(LOW) 메시지 출력
 * 참고: 폴링 방식이라 빠르게 돌리면 펄스를 놓치거나 채터링으로 여러 줄이 나올 수 있음.
 *       방향이 반대로 보이면 CLK/DT 배선을 바꾸거나 판별 조건을 뒤집으면 됨.
 *
 * 수정일: 2026-04-15
 * 변경: 최초 작성 — HW-040 회전·버튼 테스트 (내부 풀업)
 */

const int CLK_PIN = 29;  // HW-040 CLK
const int DT_PIN = 28;   // HW-040 DT
const int SW_PIN = 27;   // HW-040 SW (축 누름 스위치)

int lastClkState;

void setup() {
  Serial.begin(9600);

  // INPUT_PULLUP: 입력 + 내부 풀업 (모듈이 오픈드레인/스위치형일 때 흔히 사용)
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DT_PIN, INPUT_PULLUP);
  pinMode(SW_PIN, INPUT_PULLUP);

  lastClkState = digitalRead(CLK_PIN);

  Serial.println(F("HW-040 test start"));
  Serial.println(F("Rotate encoder or press button"));
}

void loop() {
  // 1) 회전 감지 — CLK 상태 변화 시 DT와 비교해 방향 판별
  int currentClkState = digitalRead(CLK_PIN);

  if (currentClkState != lastClkState) {
    if (digitalRead(DT_PIN) != currentClkState) {
      Serial.println(F("Clockwise"));
    } else {
      Serial.println(F("CounterClockwise"));
    }
  }

  lastClkState = currentClkState;

  // 2) 버튼 감지 — 눌림은 보통 LOW (풀업이므로 누르면 GND에 당김)
  if (digitalRead(SW_PIN) == LOW) {
    Serial.println(F("Button pressed"));
    delay(200);  // 간단한 디바운스
  }
}
