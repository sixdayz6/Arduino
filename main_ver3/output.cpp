#include "output.h"

#define SPEAKER_PIN 25
#define LEDSTRIP_PIN 27

void initOutput() {
    pinMode(SPEAKER_PIN, OUTPUT);
    pinMode(LEDSTRIP_PIN, OUTPUT);
    deactivateOutput();
    Serial.println("Output Initialized.");
}

void activateOutput() {
    tone(SPEAKER_PIN, 1000, 500);  // 스피커 소리
    for (int i = 0; i < 5; i++) { // LED 깜박임
        digitalWrite(LEDSTRIP_PIN, HIGH);
        delay(250);
        digitalWrite(LEDSTRIP_PIN, LOW);
        delay(250);
    }
}

void deactivateOutput() {
    noTone(SPEAKER_PIN);
    digitalWrite(LEDSTRIP_PIN, LOW);
}

void setSpeakerTone(int frequency) {
    tone(SPEAKER_PIN, frequency);
    delay(500); // 톤 재생 지속 시간
    noTone(SPEAKER_PIN);
}

