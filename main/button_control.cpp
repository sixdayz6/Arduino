#include "button_control.h"
#include <Arduino.h>

// 버튼 핀 번호 설정
const int plusButtonPin = 32;
const int minusButtonPin = 33;

unsigned long buttonPressStartTime = 0;
const unsigned long buttonHoldTime = 3000; // 3초 (3000밀리초)

// 버튼 초기화 함수
void setupButtons() {
    pinMode(plusButtonPin, INPUT_PULLUP);
    pinMode(minusButtonPin, INPUT_PULLUP);
}

// 버튼 중 하나가 눌렸는지 확인
bool isAnyButtonPressed() {
    return digitalRead(plusButtonPin) == LOW || digitalRead(minusButtonPin) == LOW;
}

// 두 버튼이 동시에 3초 이상 눌렸는지 확인
bool areBothButtonsHeld() {
    if (digitalRead(plusButtonPin) == LOW && digitalRead(minusButtonPin) == LOW) {
        if (buttonPressStartTime == 0) {
            buttonPressStartTime = millis(); // 버튼 눌린 시작 시간 기록
        } else if (millis() - buttonPressStartTime >= buttonHoldTime) {
            buttonPressStartTime = 0; // 타이머 초기화
            return true; // 두 버튼이 3초 이상 눌림
        }
    } else {
        buttonPressStartTime = 0; // 버튼이 동시에 눌리지 않으면 초기화
    }
    return false;
}
