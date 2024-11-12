#include "led_strip_control.h"
#include <Adafruit_NeoPixel.h>

#define ALARM_LED_PIN 6  // LED 스트립 핀 번호
#define ALARM_NUM_LEDS 12 // LED 개수

Adafruit_NeoPixel alarmStrip = Adafruit_NeoPixel(ALARM_NUM_LEDS, ALARM_LED_PIN, NEO_GRB + NEO_KHZ800);

void setupAlarmLEDStrip() {
    alarmStrip.begin();
    alarmStrip.show(); // 모든 LED를 꺼진 상태로 초기화
}

void activateAlarmLEDStrip() {
    for (int i = 0; i < ALARM_NUM_LEDS; i++) {
        alarmStrip.setPixelColor(i, alarmStrip.Color(255, 0, 0)); // 빨간색으로 LED 설정
    }
    alarmStrip.show();
}

void deactivateAlarmLEDStrip() {
    for (int i = 0; i < ALARM_NUM_LEDS; i++) {
        alarmStrip.setPixelColor(i, alarmStrip.Color(0, 0, 0)); // LED 끄기
    }
    alarmStrip.show();
}
