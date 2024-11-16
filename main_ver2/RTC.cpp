#include "RTC.h"

RTC_DS3231 rtc;

void initRTC() {
    if (!rtc.begin()) {
        Serial.println("RTC module not found.");
        while (1);
    }
    if (rtc.lostPower()) {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

DateTime getCurrentTime() {
    return rtc.now();
}

void setRTC(DateTime newTime) {
    rtc.adjust(newTime);
}
