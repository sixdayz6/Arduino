#ifndef BUTTON_CONTROL_H
#define BUTTON_CONTROL_H

// 버튼 핀 번호 설정
extern const int plusButtonPin;
extern const int minusButtonPin;


// 함수 선언
void setupButtons();
bool isAnyButtonPressed();
bool areBothButtonsHeld();

#endif // BUTTON_CONTROL_H
