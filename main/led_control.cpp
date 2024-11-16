#include "led_control.h"
#include <Adafruit_NeoPixel.h>

// Adafruit_NeoPixel 객체 생성
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// LED 스트립 설정 함수 구현
void setupLEDStrip() {
    strip.begin(); // 스트립 초기화
    strip.show();  // 모든 LED를 꺼진 상태로 초기화
}

// LED 스트립 켜기 함수 구현 (모든 LED를 하얀색으로 설정)
void turnOnLEDStrip() {
    for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(255, 255, 255)); // 모든 LED를 흰색으로 설정
    }
    strip.show();
}

// LED 스트립 끄기 함수 구현
void turnOffLEDStrip() {
    for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0)); // 모든 LED를 끔
    }
    strip.show();
}

// LED 스트립 색상 설정 함수 구현
void setLEDStripColor(uint32_t color) {
    for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, color); // 각 LED에 주어진 색상 적용
    }
    strip.show();
}
