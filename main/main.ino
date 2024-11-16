#include "rtc_control.h"
#include "ble_control.h"
#include "alarm_control.h"

unsigned long lastAlarmTime = 0; // 마지막 알람 시간 기록
const unsigned long alarmInterval = 60000; // 1분 (60000밀리초)

void setup() {
    Serial.begin(115200);
    // setupRTC();
    setupSpeaker();
    setupVibrationMotor(); // 진동 모터 설정 추가
    setupBLE();
}


void loop() {
    // 현재 시간을 가져옴
    unsigned long currentTime = millis();

    // 알람 시간이 도달했는지 확인
    if (checkAlarmTime()) {
        if (currentTime - lastAlarmTime >= alarmInterval) { // 마지막 알람 이후 1분이 경과했는지 확인
            Serial.println("알람 시간이 도달했습니다!");
            playMelody(); // 알람 재생
            Serial.print("알람 횟수: ");
            Serial.println(alarmCount); // 알람 횟수 출력
            lastAlarmTime = currentTime; // 마지막 알람 시간 업데이트
        }
    }

    // START_ALARM 명령 처리
    if (startAlarmFlag) {
        playMelody(); // 알람 재생
        startAlarmFlag = false; // 플래그 리셋
    }

    // 1초마다 시간 체크 (delay 대신 비차단 방식으로 구현해도 가능)
    delay(1000);
}

