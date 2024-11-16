#ifndef RTC_H
#define RTC_H

#include <RTClib.h>

void initRTC();
DateTime getCurrentTime();
void setRTC(DateTime newTime);

#endif
