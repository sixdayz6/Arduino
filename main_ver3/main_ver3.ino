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
    // initRTC();      // RTC 모듈 초기화
    initBluetooth();      // BLE 초기화
    initButtons();  // 버튼 핸들러 초기화
    initOutput();   // 출력 디바이스 초기화
    loadAlarms();   // 저장된 알람 불러오기

    // showTestMenu(); // 테스트가 필요할 경우 모듈 초기화를 주석처리하고 이 함수 주석 풀고 loop에 테스트 함수 loop 주석 풀기

    Serial.println("System Initialized");
}

void loop() {
    checkButtons();     // 버튼 상태 확인 및 처리
    checkAlarms();      // 알람 관리

//     // 테스트 함수 loop
//     if (Serial.available() > 0) {
//         String command = Serial.readStringUntil('\n');
//         command.trim(); // 공백 제거
//         handleTestCommand(command);
//         showTestMenu();
// }
}
