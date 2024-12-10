#ifndef OUTPUT_H
#define OUTPUT_H

#include <Arduino.h>

// extern volatile bool alarmActive; // 알람 활성화 플래그
// extern volatile bool stopAlarm;   // 알람 비활성화 플래그

// 함수 선언
void initOutput();         // 출력 디바이스 초기화
void activateOutput();     // 알림 활성화
void deactivateOutput();   // 알림 비활성화
void setSpeakerTone(int frequency); // 출력 장치 초기화 및 제어 함수
void searchingBluethoot(); // 블루투스 찾는 동안 led 작동
void blue_button1_led();
void red_button2_led();


#endif // OUTPUT_H
