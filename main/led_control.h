#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <Adafruit_NeoPixel.h>

// 핀 번호 및 LED 개수 정의
const int LED_PIN = 2;
const int NUM_LEDS = 12;

// 함수 선언
void setupLEDStrip();
void turnOnLEDStrip();
void turnOffLEDStrip();
void setLEDStripColor(uint32_t color);

#endif // LED_CONTROL_H
