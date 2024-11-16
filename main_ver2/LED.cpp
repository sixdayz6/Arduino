#include "LED.h"
#include <Adafruit_NeoPixel.h>

#define LED_PIN 5
#define NUM_LEDS 10

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void initLED() {
    strip.begin();
    strip.show();
}

void startLED() {
    for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(255, 0, 0)); // Red
    }
    strip.show();
}

void stopLED() {
    strip.clear();
    strip.show();
}
