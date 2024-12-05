#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// 공통 상수
#define LONG_PRESS_THRESHOLD 2000 // 2초 이상은 길게 누름
#define MIN_ALARM_INTERVAL 1     // 최소 알람 간격 (분)
#define MAX_ALARM_INTERVAL 1440   // 최대 알람 간격 (분)

// 공통 전역 변수
extern bool alarmActive; // 현재 알람 활성 상태
extern bool bleSearching; // 블루투스 찾는 중 플래그

// 공통 구조체
struct Alarm {
    uint32_t interval;    // 알람 간격 (분)
    uint32_t nextTrigger; // 다음 알람 시간 (Unix timestamp)
};

extern Alarm currentAlarm; // 현재 알람 데이터

#endif // CONFIG_H
