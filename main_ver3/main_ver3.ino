#include "config.h"
#include "alarm.h"
#include "bluetooth.h"
#include "rtc.h"
#include "buttons.h"
#include "output.h"
#include "tests.h"

bool alarmActive = false; // 전역 변수 정의

void setup() {
    Serial.begin(115200);

    // 모듈 초기화
    initRTC();      // RTC 모듈 초기화
    initBLE();      // BLE 초기화
    initButtons();  // 버튼 핸들러 초기화
    initOutput();   // 출력 디바이스 초기화
    loadAlarms();   // 저장된 알람 불러오기

    showTestMenu();

    Serial.println("System Initialized");
}

void loop() {
    // checkButtons();     // 버튼 상태 확인 및 처리
    // checkAlarms();      // 알람 관리

    // Serial Monitor에서 명령 처리
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim(); // 공백 제거
        handleTestCommand(command);
        showTestMenu();
}
}
