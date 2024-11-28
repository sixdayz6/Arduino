#include <Adafruit_NeoPixel.h>

// 설정 값
#define LED_PIN 15      // LED 데이터 핀이 연결된 ESP32의 GPIO 핀
#define NUM_LEDS 50     // LED 개수

// Adafruit NeoPixel 객체 생성
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  strip.begin();       // LED 스트립 초기화
  strip.show();        // 모든 LED를 꺼줌
  Serial.begin(115200); // 디버그용 시리얼 통신 시작
}

void loop() {
  // 1. 순차적으로 LED를 켜는 효과
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0, 0)); // 빨간색
    strip.show();
    delay(100); // 100ms 대기
  }

  // 2. 순차적으로 LED를 끄는 효과
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0, 0)); // LED 끄기
    strip.show();
    delay(100); // 100ms 대기
  }

  // 3. 모든 LED를 녹색으로 점등 후 1초 유지
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 255, 0, 0)); // 녹색
  }
  strip.show();
  delay(1000); // 1초 대기

  // 4. 모든 LED를 끔
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0, 0));
  }
  strip.show();
  delay(1000); // 1초 대기
}
