#include "buttons.h"
#include "alarm.h"
#include "bluetooth.h"
#include "config.h"

#define BUTTON1_PIN 12
#define BUTTON2_PIN 14

unsigned long button1PressTime = 0;
unsigned long button2PressTime = 0;
bool button1Pressed = false;
bool button2Pressed = false;
bool actionProcessed = false;

void initButtons() {
    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);
    Serial.println("Buttons Initialized.");
}

void checkButtons() {
    // 현재 버튼 상태 읽기
    bool currentButton1State = digitalRead(BUTTON1_PIN) == LOW;
    bool currentButton2State = digitalRead(BUTTON2_PIN) == LOW;

    // 버튼1 처리
    if (currentButton1State && !button1Pressed) {
        button1PressTime = millis();
        button1Pressed = true;
    } else if (!currentButton1State && button1Pressed) {
        // 버튼1 짧게 눌림 처리
        if (!button2Pressed && !actionProcessed) {
            if (millis() - button1PressTime < LONG_PRESS_THRESHOLD) {
                handleButton1ShortPress();
            }
        }
        button1Pressed = false;
        actionProcessed = false; // 처리 완료 후 플래그 리셋
    }

    // 버튼1 길게 눌림 처리
    if (button1Pressed && !actionProcessed) {
        if (millis() - button1PressTime > LONG_PRESS_THRESHOLD) {
            handleButton1LongPress();
            actionProcessed = true; // 길게 눌림 처리 후 추가 실행 방지
        }
    }

    // 버튼2 처리
    if (currentButton2State && !button2Pressed) {
        button2PressTime = millis();
        button2Pressed = true;
    } else if (!currentButton2State && button2Pressed) {
        // 버튼2 짧게 눌림 처리
        if (!button1Pressed && !actionProcessed) {
            if (millis() - button2PressTime < LONG_PRESS_THRESHOLD) {
                handleButton2ShortPress();
            }
        }
        button2Pressed = false;
        actionProcessed = false; // 처리 완료 후 플래그 리셋
    }

    // 버튼2 길게 눌림 처리
    if (button2Pressed && !actionProcessed) {
        if (millis() - button2PressTime > LONG_PRESS_THRESHOLD) {
            handleButton2LongPress();
            actionProcessed = true; // 길게 눌림 처리 후 추가 실행 방지
        }
    }

    // 두 버튼 동시에 눌림 처리
    // if (button1Pressed && button2Pressed) {
    //     if (!actionProcessed) {
    //         if (millis() - button1PressTime > LONG_PRESS_THRESHOLD &&
    //             millis() - button2PressTime > LONG_PRESS_THRESHOLD) {
    //             handleBothButtonsLongPress();
    //         } else if (millis() - button1PressTime < LONG_PRESS_THRESHOLD &&
    //                    millis() - button2PressTime < LONG_PRESS_THRESHOLD) {
    //             handleBothButtonsShortPress();
    //         }
    //         actionProcessed = true; // 두 버튼 동작 처리 후 추가 실행 방지
    //     }
    // }
}

// 버튼 동작 함수 정의
void handleButton1ShortPress() {
    adjustAlarmInterval(30);
    Serial.println("Button 1 Short Press: Alarm interval increased by 30 minutes.");
}

void handleButton1LongPress() {
    startCountdown();
    Serial.println("Button 1 Long Press: Alarm countdown started.");
}

void handleButton2ShortPress() {
    adjustAlarmInterval(-30);
    Serial.println("Button 2 Short Press: Alarm interval decreased by 30 minutes.");
}

void handleButton2LongPress() {
    stopCurrentAlarm();
    Serial.println("Button 2 Long Press: Current alarm stopped.");
}

void handleBothButtonsShortPress() {
    resetAlarms();
    Serial.println("Both Buttons Short Press: Alarms reset, all alarms deleted.");
}

void handleBothButtonsLongPress() {
    startBLEAdvertising();
    Serial.println("Both Buttons Long Press: BLE advertising started.");
}
