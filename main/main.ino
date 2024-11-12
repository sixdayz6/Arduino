#include "rtc_control.h"
#include "ble_control.h"
#include "alarm_control.h"
#include "led_strip_control.h"
#include "button_control.h"
#include "alarm_adjust.h"

unsigned long lastAlarmTime = 0; // 다음 알람 시간 계산을 위해서 마지막 알람 시간을 저장

bool isAlarmActive = false; // 버튼으로 알람을 끄기 위해 알람이 켜져 있는지 확인
bool isSleepMode = false; // sleep 모드 여부 확인
bool isTimerAdjustMode = false; // 타이머 조절 모드 상태

void setup() {
    Serial.begin(115200);
    setupRTC(); // 1
    setupSpeaker(); // 2
    setupVibrationMotor(); // 3
    setupAlarmLEDStrip(); // 4
    setupBLE(); // 5
    setupButtons(); // 6 버튼 초기화
    setupAlarmAdjust(); // 7
}

void loop() {
    unsigned long currentTime = millis();
    unsigned long alarmIntervalMillis = getAlarmIntervalInMinutes() * 60000; // 알람 간격을 밀리초로 변환

    if (!isTimerAdjustMode) {
        // 알람이 활성화된 경우 버튼을 눌러 알람 끄기
        if (isAlarmActive && isAnyButtonPressed()) {
            stopAlarm();
        }

        // 알람 간격이 경과했는지 확인하여 알람 실행
        if (currentTime - lastAlarmTime >= alarmIntervalMillis) {
            Serial.println("알람 간격이 도달했습니다!");
            playMelody();
            activateAlarmLEDStrip();
            isAlarmActive = true;
            lastAlarmTime = currentTime;
        }


    } else {
        adjustAlarmInterval(); // 타이머 조절 모드에서 알람 간격 조정

        // 조정 완료 후 타이머 조절 모드 비활성화
        if (!isAnyButtonPressed()) { // 버튼을 모두 놓으면 조정 종료
            isTimerAdjustMode = false;
        }
    }

    // 알람이 활성화된 경우 버튼을 눌러 알람 끄기
    if (isAlarmActive && isAnyButtonPressed()) {
        stopAlarm(); // 알람 종료
      }

    // START_ALARM 명령 처리
    if (startAlarmFlag) {
        playMelody();
        activateAlarmLEDStrip();
        isAlarmActive = true;
        startAlarmFlag = false;
      }

    // 타이머 조절 모드 진입 조건 확인
    if (!isSleepMode && areBothButtonsHeld()) {
        isTimerAdjustMode = true;
        Serial.println("타이머 조절 모드에 진입했습니다.");
    }

    delay(1000); // 1초마다 루프 실행
}

// 알람 종료 함수
void stopAlarm() {
    isAlarmActive = false;
    noTone(SPEAKER_PIN);
    deactivateAlarmLEDStrip();
    Serial.println("알람이 종료되었습니다.");
}

