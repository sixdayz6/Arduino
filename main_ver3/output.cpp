#include "output.h"
#include "config.h"
#include "alarm.h"
#include <Adafruit_NeoPixel.h>


#define SPEAKER_PIN 25
#define LEDSTRIP_PIN 27
#define VIBRATION_MOTOR_PIN 4  // Use any PWM-capable GPIO pin
#define PWM_CHANNEL 0           // LEDC PWM channel (0-15)
#define PWM_FREQ 5000           // PWM frequency (in Hz)
#define PWM_RESOLUTION 8        // PWM resolution (8 bits: 0-255 duty cycle)

#define NUM_LEDS 24

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LEDSTRIP_PIN, NEO_GRBW + NEO_KHZ800);

// volatile bool alarmActive = false; // Tracks whether the alarm is active
// volatile bool stopAlarm = false;   // Tracks whether the alarm should stop


void initOutput() {
    pinMode(SPEAKER_PIN, OUTPUT);
    pinMode(LEDSTRIP_PIN, OUTPUT);

    // // Configure the PWM channel
    // ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    // ledcAttachPin(VIBRATION_MOTOR_PIN, PWM_CHANNEL);

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
            // longBuzz();
            delay(500);

            if (!alarmActive) {
                deactivateOutput();
                break; // Check again for stop signal
            }
        }
    }
    deactivateOutput();
}

void deactivateOutput() {
    if (!alarmActive) return; // Do nothing if the alarm is not active
    alarmActive = false;
    noTone(SPEAKER_PIN);
    strip.clear(); // Clear all LEDs
    strip.show();
    startCountdown();
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

// void shortBuzz() {
//     ledcWrite(PWM_CHANNEL, 200); // Medium intensity
//     delay(200);                  // Vibrate for 200ms
//     ledcWrite(PWM_CHANNEL, 0);   // Turn off
//     delay(100);                  // Pause
// }

// void longBuzz() {
//     ledcWrite(PWM_CHANNEL, 255); // Full intensity
//     delay(500);                  // Vibrate for 500ms
//     ledcWrite(PWM_CHANNEL, 0);   // Turn off
// }


