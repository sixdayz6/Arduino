#ifndef ALARM_H
#define ALARM_H

#include <RTClib.h>
#include <vector>

struct Alarm {
    DateTime alarmTime;
    bool isActive;
};

void loadAlarms();  // EEPROM에서 알람 로드
void saveAlarms();  // 알람 저장
void checkAlarm();  // 알람 확인
void triggerAlarm(); // 알람 작동
void stopAlarm();    // 알람 정지

#endif
