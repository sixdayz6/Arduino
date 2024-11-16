#include "Speaker.h"

#define SPEAKER_PIN 27

void initSpeaker() {
    pinMode(SPEAKER_PIN, OUTPUT);
}

void startSpeaker() {
    tone(SPEAKER_PIN, 1000); // 1kHz 소리
}

void stopSpeaker() {
    noTone(SPEAKER_PIN);
}
