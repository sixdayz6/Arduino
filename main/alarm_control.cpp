#include "alarm_control.h"
#include <Arduino.h>

const int SPEAKER_PIN = 25; // 스피커 핀 번호
const int VIBRATION_PIN = 26; // 진동 모터 핀 번호

// 멜로디 음계와 길이 배열
int melody[] = {
    262, 294, 330, 349, 392, 440, 494, 523 // C, D, E, F, G, A, B, C
};

int noteDurations[] = {
    4, 4, 4, 4, 4, 4, 4, 4 // 각 음의 길이 (4분 음표)
};

void setupSpeaker() {
    pinMode(SPEAKER_PIN, OUTPUT);
}

void setupVibrationMotor() {
    pinMode(VIBRATION_PIN, OUTPUT);
    digitalWrite(VIBRATION_PIN, LOW); // 초기에는 비활성화
}

void playAlarm() {
    // 기본적인 알람 소리와 진동
    for (int i = 0; i < 3; i++) {
        digitalWrite(SPEAKER_PIN, HIGH);
        activateVibrationMotor(); // 진동 모터 활성화
        delay(500);
        digitalWrite(SPEAKER_PIN, LOW);
        deactivateVibrationMotor(); // 진동 모터 비활성화
        delay(500);
    }
}

void playMelody() {
    for (int thisNote = 0; thisNote < 8; thisNote++) {
        int noteDuration = 1000 / 4; // 4분 음표 기준
        tone(SPEAKER_PIN, melody[thisNote], noteDuration);
        activateVibrationMotor(); // 진동 모터 활성화
        delay(noteDuration * 1.3);
        noTone(SPEAKER_PIN);
        deactivateVibrationMotor(); // 진동 모터 비활성화
    }
}

void activateVibrationMotor() {
    digitalWrite(VIBRATION_PIN, HIGH);
}

void deactivateVibrationMotor() {
    digitalWrite(VIBRATION_PIN, LOW);
}
