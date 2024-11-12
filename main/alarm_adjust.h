#ifndef ALARM_ADJUST_H
#define ALARM_ADJUST_H

// 알람 간격 관련 변수 및 함수 선언
extern bool isTimerAdjustMode; // 외부에서 접근 가능한 타이머 조절 모드 변수
void setupAlarmAdjust();
void enableTimerAdjustMode();
void adjustAlarmInterval();
int getAlarmIntervalInMinutes();

#endif // ALARM_ADJUST_H
