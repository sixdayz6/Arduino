#include "Buttons.h"
#include "Alarm.h"

#define BUTTON1_PIN 12
#define BUTTON2_PIN 13

#define LONG_PRESS_TIME 1000

void initButtons() {
    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);
}

void handleButtonActions() {
    static unsigned long button1PressTime = 0;
    static unsigned long button2PressTime = 0;

    // 버튼 1 처리
    if (digitalRead(BUTTON1_PIN) == LOW) {
        if (button1PressTime == 0) button1PressTime = millis();
        if (millis() - button1PressTime > LONG_PRESS_TIME) {
            // 긴 누름
            Serial.println("Button 1 long press");
            // Alarm 저장 및 카운트다운 시작
        }
    } else {
        if (button1PressTime > 0 && millis() - button1PressTime < LONG_PRESS_TIME) {
            // 짧게 누름
            Serial.println("Button 1 short press");
            // 알람 간격 증가
        }
        button1PressTime = 0;
    }

    // 버튼 2 처리
    if (digitalRead(BUTTON2_PIN) == LOW) {
        if (button2PressTime == 0) button2PressTime = millis();
        if (millis() - button2PressTime > LONG_PRESS_TIME) {
            // 긴 누름
            Serial.println("Button 2 long press");
            stopAlarm();
        }
    } else {
        if (button2PressTime > 0 && millis() - button2PressTime < LONG_PRESS_TIME) {
            // 짧게 누름
            Serial.println("Button 2 short press");
            // 알람 간격 감소
        }
        button2PressTime = 0;
    }
}
