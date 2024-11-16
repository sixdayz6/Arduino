#include "config.h"
#include "alarm.h"
#include "rtc.h"
#include "output.h"
#include "bluetooth.h"

Alarm currentAlarm = {60, 0}; // 초기값: 간격 60분, 트리거 없음

void loadAlarms() {
    // EEPROM이나 SPIFFS에서 저장된 알람 읽기 (가상 코드)
    // 현재는 초기값 설정
    currentAlarm.interval = 60;
    currentAlarm.nextTrigger = 0;
    Serial.println("Alarms loaded.");
}

void saveAlarms() {
    // EEPROM이나 SPIFFS에 알람 저장 (가상 코드)
    Serial.println("Alarms saved.");
}

void adjustAlarmInterval(int32_t delta) {
    currentAlarm.interval = constrain(currentAlarm.interval + delta, MIN_ALARM_INTERVAL, MAX_ALARM_INTERVAL);
    Serial.printf("Alarm Interval Adjusted: %d minutes\n", currentAlarm.interval);
}

void startCountdown() {
    currentAlarm.nextTrigger = getRTC() + currentAlarm.interval * 60;
    saveAlarms();
    Serial.printf("Countdown started. Next trigger at %d\n", currentAlarm.nextTrigger);
}

void checkAlarms() {
    if (alarmActive || currentAlarm.nextTrigger == 0) {
        return;
    }

    uint32_t now = getRTC();
    if (now >= currentAlarm.nextTrigger) {
        triggerAlarm();
    }
}

void triggerAlarm() {
    Serial.println("Alarm Triggered!");
    alarmActive = true;
    activateOutput();       // 스피커, LED 등 알림 활성화
    notifyBLE("Alarm Activated");
}

void stopCurrentAlarm() {
    if (alarmActive) {
        deactivateOutput(); // 알림 비활성화
        alarmActive = false;
        startCountdown();   // 다음 알람 카운트다운 시작
    }
}

void resetAlarms() {
    currentAlarm.interval = 0;
    currentAlarm.nextTrigger = 0;
    saveAlarms();
    alarmActive = false;
    deactivateOutput();
    Serial.println("All alarms reset.");
}
