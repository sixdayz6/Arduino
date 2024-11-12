#include "alarm_control.h"
#include <Arduino.h>

const int SPEAKER_PIN = 25; // 스피커 핀 번호
const int VIBRATION_PIN = 26; // 진동 모터 핀 번호

int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};
int noteDurations[] = {4, 4, 4, 4, 4, 4, 4, 4};

void setupSpeaker() {
    pinMode(SPEAKER_PIN, OUTPUT);
}

void setupVibrationMotor() {
    pinMode(VIBRATION_PIN, OUTPUT);
    digitalWrite(VIBRATION_PIN, LOW);
}

void playAlarm() {
    for (int i = 0; i < 3; i++) {
        digitalWrite(SPEAKER_PIN, HIGH);
        activateVibrationMotor();
        delay(500);
        digitalWrite(SPEAKER_PIN, LOW);
        deactivateVibrationMotor();
        delay(500);
    }
}

void playMelody() {
    for (int thisNote = 0; thisNote < 8; thisNote++) {
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(SPEAKER_PIN, melody[thisNote], noteDuration);
        activateVibrationMotor();
        delay(noteDuration * 1.3);
        noTone(SPEAKER_PIN);
        deactivateVibrationMotor();
    }
}

void activateVibrationMotor() {
    digitalWrite(VIBRATION_PIN, HIGH);
}

void deactivateVibrationMotor() {
    digitalWrite(VIBRATION_PIN, LOW);
}
