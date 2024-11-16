#include "Alarm.h"
#include "Speaker.h"
#include "LED.h"

std::vector<Alarm> alarms;

void loadAlarms() {
    // EEPROM에서 알람 로드 (간단히 구현)
    Serial.println("Alarms loaded from EEPROM.");
}

void saveAlarms() {
    // 알람을 EEPROM에 저장
    Serial.println("Alarms saved to EEPROM.");
}

void checkAlarm() {
    DateTime now = rtc.now();
    for (auto &alarm : alarms) {
        if (alarm.isActive && now >= alarm.alarmTime) {
            triggerAlarm();
            alarm.isActive = false; // 알람 비활성화
            saveAlarms();
        }
    }
}

void triggerAlarm() {
    startSpeaker();
    startLED();
    Serial.println("Alarm triggered!");
}

void stopAlarm() {
    stopSpeaker();
    stopLED();
    Serial.println("Alarm stopped!");
}
