#include <RTClib.h>
#include "rtc.h"

RTC_DS3231 rtc;

void initRTC() {
    if (!rtc.begin()) {
        Serial.println("RTC module not found!");
        while (1); // 중단
    }

    if (rtc.lostPower()) {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // 현재 시간 설정
    }

    Serial.println("RTC Initialized.");
}

uint32_t getRTC() {
    DateTime now = rtc.now();
    return now.unixtime();
}

String getFormattedTime() {
    DateTime now = rtc.now();
    char buffer[20];
    sprintf(buffer, "%02d:%02d:%02d %02d/%02d/%04d", 
        now.hour(), now.minute(), now.second(), 
        now.day(), now.month(), now.year());
    return String(buffer);
}
