#ifndef RTC_CONTROL_H
#define RTC_CONTROL_H

#include <Wire.h>
#include <RTClib.h>

extern RTC_DS3231 rtc;
extern int alarmCount; // 알람 횟수 변수

// 함수 선언
void setupRTC();
bool checkAlarmTime();
void setAlarmTime(int hour, int minute); // 알람 시간 설정

#endif // RTC_CONTROL_H
