#include "rtc_control.h"

RTC_DS3231 rtc;
int alarmHour = 0;
int alarmMinute = 0;
int alarmCount = 0; // 알람 횟수 변수 정의

void setupRTC() {
    if (!rtc.begin()) {
        Serial.println("RTC를 초기화할 수 없습니다.");
        while (1);
    }
    if (rtc.lostPower()) {
        Serial.println("RTC의 시간이 초기화되었습니다. 현재 시간을 설정합니다.");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    Serial.println("RTC가 성공적으로 초기화되었습니다.");
}

bool checkAlarmTime() {
    DateTime now = rtc.now();
    if (now.hour() == alarmHour && now.minute() == alarmMinute && now.second() == 0) {
        alarmCount++; // 알람이 울릴 때마다 횟수 증가
        return true;
    }
    return false;
}

void setAlarmTime(int hour, int minute) {
    alarmHour = hour;
    alarmMinute = minute;
}
