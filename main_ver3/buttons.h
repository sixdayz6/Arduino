#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

// 함수 선언
void initButtons();       // 버튼 초기화
void checkButtons();      // 버튼 상태 확인
void turnOffButton();

// 버튼 이벤트 핸들러 선언
void handleButton1ShortPress();
void handleButton1LongPress();
void handleButton2ShortPress();
void handleButton2LongPress();
void handleBothButtonsShortPress();
void handleBothButtonsLongPress();

#endif // BUTTONS_H
