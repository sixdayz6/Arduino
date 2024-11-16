#ifndef RTC_H
#define RTC_H

#include <Arduino.h>

// 함수 선언
void initRTC();            // RTC 초기화
uint32_t getRTC();         // 현재 RTC 시간 (Unix timestamp) 가져오기
String getFormattedTime(); // 포맷된 시간 반환

#endif // RTC_H
