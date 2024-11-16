#include "tests.h"
#include "output.h"
#include "buttons.h"
#include "alarm.h"
#include "bluetooth.h"
#include "config.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

BLEServer *bleServer = nullptr;
bool bleDeviceConnected = false;
unsigned long bleStartTime = 0;

// UUID 정의
#define SERVICE_UUID           "12345678-1234-1234-1234-123456789abc"
#define CHARACTERISTIC_UUID_TX "abcd1234-1234-1234-1234-123456789abc"

// BLE 콜백 클래스
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *server) override {
        bleDeviceConnected = true;
        Serial.println("Device connected.");
    }

    void onDisconnect(BLEServer *server) override {
        bleDeviceConnected = false;
        Serial.println("Device disconnected.");
    }
};

void initBLEForTest() {
    BLEDevice::init("ESP32_Alarm_Test");
    bleServer = BLEDevice::createServer();
    bleServer->setCallbacks(new MyServerCallbacks());

    BLEService *service = bleServer->createService(SERVICE_UUID);

    BLECharacteristic *txCharacteristic = service->createCharacteristic(
        CHARACTERISTIC_UUID_TX,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );

    txCharacteristic->setValue("Hello from ESP32");
    service->start();

    BLEAdvertising *advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(SERVICE_UUID);
    advertising->setScanResponse(true);
    advertising->start();

    Serial.println("BLE Advertising started.");
    bleStartTime = millis();
}


// 테스트 메뉴 출력
void showTestMenu() {
    Serial.println("\n=== TEST MENU ===");
    Serial.println("1: Test Speaker");
    Serial.println("2: Test Buttons");
    Serial.println("3: Test Alarm Interval");
    Serial.println("4: Test Bluetooth Connection");
    Serial.println("==================");
    Serial.println("Enter your choice: ");
}

// 테스트 명령 처리
void handleTestCommand(String command) {
    if (command == "1") {
        testSpeaker();
    } else if (command == "2") {
        testButtons();
    } else if (command == "3") {
        testAlarmInterval();
    } else if (command == "4") {
        testBluetooth();
    } else {
        Serial.println("Invalid choice! Please try again.");
    }
}

// 1. 스피커 테스트
void testSpeaker() {
    Serial.println("Testing Speaker...");
    activateOutput(); // 스피커, LED 등 활성화
    delay(1000);      // 1초 동안 활성화 상태 유지
    deactivateOutput();
    Serial.println("Speaker test complete.");
}

// 2. 버튼 테스트
void testButtons() {
    Serial.println("Testing Buttons...");
    Serial.println("Press Button 1 or Button 2 to see the output.");
    unsigned long startTime = millis();

    while (millis() - startTime < 10000) { // 10초 동안 버튼 테스트
        checkButtons(); // 버튼 상태 확인
        if (digitalRead(12) == LOW) {
            Serial.println("Button 1 Pressed.");
            delay(200); // 디바운싱 딜레이
        }
        if (digitalRead(14) == LOW) {
            Serial.println("Button 2 Pressed.");
            delay(200); // 디바운싱 딜레이
        }
    }

    Serial.println("Button test complete.");
}

// 3. 알람 테스트
void testAlarmInterval() {
    Serial.println("Testing Alarm Interval...");
    currentAlarm.interval = 10 / 60; // 10초 간격 설정
    startCountdown();
    Serial.println("Alarm set to trigger in 10 seconds.");

    unsigned long startTime = millis();
    while (millis() - startTime < 15000) { // 15초 동안 알람 테스트
        checkAlarms(); // 알람 상태 확인
        delay(100);    // 간단한 주기
    }

    resetAlarms(); // 알람 초기화
    Serial.println("Alarm test complete and reset.");
}

// 4. 블루투스 연결 테스트
void testBluetooth() {
    Serial.println("Press Button 1 to start Bluetooth advertising...");

    // 1분 타이머 설정
    unsigned long timeout = 60000; // 1분

    while (millis() - bleStartTime < timeout) {
        checkButtons(); // 버튼 상태 확인

        if (digitalRead(12) == LOW) {
            initBLEForTest();
            break;
        }

        if (bleDeviceConnected) {
            // 연결된 상태면 디바이스 정보 출력
            Serial.println("Device connected successfully. Test complete.");
            return;
        }

        delay(100); // CPU 부하 방지
    }

    if (!bleDeviceConnected) {
        Serial.println("No device connected within 1 minute. Test timed out.");
    }
}
