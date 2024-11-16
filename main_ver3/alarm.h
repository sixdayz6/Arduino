#ifndef ALARM_H
#define ALARM_H

#include <Arduino.h>

// 함수 선언
void loadAlarms();          // 알람 불러오기
void saveAlarms();          // 알람 저장
void adjustAlarmInterval(int32_t delta); // 알람 간격 조정
void startCountdown();      // 알람 카운트다운 시작
void checkAlarms();         // 알람 상태 확인
void triggerAlarm();        // 알람 실행
void stopCurrentAlarm();    // 현재 알람 정지
void resetAlarms();         // 모든 알람 초기화

#endif // ALARM_H
