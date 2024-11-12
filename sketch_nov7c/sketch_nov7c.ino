#include <Adafruit_NeoPixel.h>

#define LED_PIN 15      // DATA 핀이 연결된 GPIO 핀
#define NUM_LEDS 12     // LED의 개수 설정

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // LED 초기화
}

void loop() {
  for(int i=0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0, 0)); // 빨간색
    strip.show();
    delay(100);
    strip.setPixelColor(i, strip.Color(0, 0, 0, 0)); // 꺼짐
    strip.show();
  }
}
