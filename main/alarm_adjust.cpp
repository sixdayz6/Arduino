#include "alarm_adjust.h"
#include "button_control.h"
#include <Arduino.h>

const int MIN_ALARM_INTERVAL = 30; // 최소 알람 간격 (분)
const int MAX_ALARM_INTERVAL = 1440; // 최대 알람 간격 (24시간 = 1440분)

int alarmIntervalInMinutes = MIN_ALARM_INTERVAL;
bool isTimerAdjustMode = false;

void setupAlarmAdjust() {
    // 필요시 초기화 작업 추가
}

void enableTimerAdjustMode() {
    isTimerAdjustMode = true;
    Serial.println("타이머 조절 모드에 진입했습니다.");
}

// 알람 간격 조정 함수
void adjustAlarmInterval() {
    if (isTimerAdjustMode) {
        if (digitalRead(plusButtonPin) == LOW) {
            alarmIntervalInMinutes += 30;
            if (alarmIntervalInMinutes > MAX_ALARM_INTERVAL) {
                alarmIntervalInMinutes = MAX_ALARM_INTERVAL;
            }
            Serial.print("알람 간격 증가: ");
            Serial.print(alarmIntervalInMinutes);
            Serial.println(" 분");
            delay(500); // 빠른 반복을 방지하기 위한 지연
        }

        if (digitalRead(minusButtonPin) == LOW) {
            alarmIntervalInMinutes -= 30;
            if (alarmIntervalInMinutes < MIN_ALARM_INTERVAL) {
                alarmIntervalInMinutes = MIN_ALARM_INTERVAL;
            }
            Serial.print("알람 간격 감소: ");
            Serial.print(alarmIntervalInMinutes);
            Serial.println(" 분");
            delay(500); // 빠른 반복을 방지하기 위한 지연
        }
    }
}

// 현재 설정된 알람 간격을 분 단위로 가져오는 함수
int getAlarmIntervalInMinutes() {
    return alarmIntervalInMinutes;
}
