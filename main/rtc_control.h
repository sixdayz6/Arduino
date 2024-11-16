#ifndef RTC_CONTROL_H
#define RTC_CONTROL_H

#include <Wire.h>
#include <RTClib.h>

extern RTC_DS3231 rtc;
extern int alarmCount; // 알람 횟수 변수 선언


// 함수 선언
void setupRTC();
bool checkAlarmTime();
void setAlarmTime(int hour, int minute); // 추가된 함수

#endif // RTC_CONTROL_H
