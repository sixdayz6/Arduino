#include "Alarm.h"
#include "Bluetooth.h"
#include "RTC.h"
#include "LED.h"
#include "Buttons.h"
#include "Speaker.h"

void setup() {
    Serial.begin(115200);

    // 모듈 초기화
    initButtons();
    initRTC();
    initBluetooth();
    initLED();
    initSpeaker();

    loadAlarms(); // 저장된 알람 로드
    Serial.println("Setup complete!");
}

void loop() {
    handleButtonActions(); // 버튼 처리
    checkAlarm();          // 알람 확인
    processBluetooth();    // BLE 처리
}
