#ifndef OUTPUT_H
#define OUTPUT_H

#include <Arduino.h>

// 함수 선언
void initOutput();         // 출력 디바이스 초기화
void activateOutput();     // 알림 활성화
void deactivateOutput();   // 알림 비활성화
void setSpeakerTone(int frequency); // 출력 장치 초기화 및 제어 함수


#endif // OUTPUT_H
