#include "config.h"
#include "alarm.h"
#include "rtc.h"
#include "output.h"
#include "bluetooth.h"
#include <Preferences.h>

Alarm currentAlarm = {60, 0}; // 초기값: 간격 60분, 트리거 없음

Preferences alarm_preferences; // Create a Preferences instance

void loadAlarms() {
    alarm_preferences.begin("AlarmPrefs", true); // Open alarm_preferences in read-only mode
    currentAlarm.interval = alarm_preferences.getInt("interval", 60); // Default interval: 60 minutes
    currentAlarm.nextTrigger = alarm_preferences.getUInt("nextTrigger", 0); // Default next trigger: 0
    alarm_preferences.end();

    Serial.printf("Alarms loaded. Interval: %d minutes, Next Trigger: %d\n",
                  currentAlarm.interval, currentAlarm.nextTrigger);
}

void saveAlarms() {
    alarm_preferences.begin("AlarmPrefs", false); // Open alarm_preferences in write mode
    alarm_preferences.putInt("interval", currentAlarm.interval);
    alarm_preferences.putUInt("nextTrigger", currentAlarm.nextTrigger);
    alarm_preferences.end();

    Serial.printf("Alarms saved. Interval: %d minutes, Next Trigger: %d\n",
                  currentAlarm.interval, currentAlarm.nextTrigger);
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
    notifyBLE("Alarm Activated");
    alarmActive = true;
    activateOutput();       // 스피커, LED 등 알림 활성화
}

void stopCurrentAlarm() {
    // if (alarmActive) {
    //     deactivateOutput(); // 알림 비활성화
    //     alarmActive = false;
    //     startCountdown();   // 다음 알람 카운트다운 시작
    // }
    deactivateOutput(); // 알림 비활성화
    alarmActive = false;
}

void resetAlarms() {
    currentAlarm.interval = 0;
    currentAlarm.nextTrigger = 0;
    saveAlarms();
    alarmActive = false;
    deactivateOutput();
    Serial.println("All alarms reset.");
}
