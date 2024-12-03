#include "output.h"
#include "config.h"
#include <Adafruit_NeoPixel.h>


#define SPEAKER_PIN 25
#define LEDSTRIP_PIN 27
#define NUM_LEDS 24

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LEDSTRIP_PIN, NEO_GRBW + NEO_KHZ800);

// volatile bool alarmActive = false; // Tracks whether the alarm is active
// volatile bool stopAlarm = false;   // Tracks whether the alarm should stop


void initOutput() {
    pinMode(SPEAKER_PIN, OUTPUT);
    pinMode(LEDSTRIP_PIN, OUTPUT);
    deactivateOutput();
    Serial.println("Output Initialized.");
}

void activateOutput() {

    uint32_t colors[] = {
        strip.Color(255, 0, 0),   // Red
        strip.Color(255, 255, 0), // Yellow
        strip.Color(0, 255, 0),   // Green
        strip.Color(0, 0, 255)    // Blue
    };

    unsigned long startTime = millis();
    while (millis() - startTime < 30000) { // Run for 30 seconds
        if (!alarmActive) {
            break; // Stop the alarm if requested
        }

        for (int i = 0; i < 4; i++) { // Cycle through colors
            strip.fill(colors[i], 0, NUM_LEDS);
            strip.show();
            tone(SPEAKER_PIN, 1000, 500); // Play sound for 500ms
            delay(500);

            if (!alarmActive) {
                break; // Check again for stop signal
            }
        }
    }

    deactivateOutput();
}

void deactivateOutput() {
    if (!alarmActive) return; // Do nothing if the alarm is not active
    noTone(SPEAKER_PIN);
    strip.clear(); // Clear all LEDs
    strip.show();
    alarmActive = false;
    Serial.println("Output Deactivated.");
}

void setSpeakerTone(int frequency) {
    tone(SPEAKER_PIN, frequency);
    delay(500); // 톤 재생 지속 시간
    noTone(SPEAKER_PIN);
}

void searchingBluethoot() {
  if (!bleSearching) return;
  const int steps = 50;            // Number of steps for brightness transition
    const int delayPerStep = 10;     // Delay per step in milliseconds (10ms for smooth transition)
    const int maxBrightness = 255;  // Maximum brightness value for blue

  while(bleSearching) {
    
    // Gradually increase brightness
    for (int brightness = 0; brightness <= maxBrightness; brightness += maxBrightness / steps) {
        uint32_t color = strip.Color(0, 0, brightness); // Blue color with variable brightness
        strip.fill(color, 0, NUM_LEDS);                // Set all LEDs to the color
        strip.show();
        delay(delayPerStep);                           // Delay to smooth transition

        if (!bleSearching) break;
    }

    // Gradually decrease brightness
    for (int brightness = maxBrightness; brightness >= 0; brightness -= maxBrightness / steps) {
        uint32_t color = strip.Color(0, 0, brightness); // Blue color with variable brightness
        strip.fill(color, 0, NUM_LEDS);                // Set all LEDs to the color
        strip.show();
        delay(delayPerStep);                           // Delay to smooth transition

        if (!bleSearching) break;

    }
  }
}

