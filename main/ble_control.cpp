#include "ble_control.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "rtc_control.h"
#include "alarm_control.h"

bool startAlarmFlag = false; // 알람 시작 플래그

BLECharacteristic *pCharacteristic;
String bleCommand = ""; // BLE 명령어 저장

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        Serial.println("BLE 클라이언트가 연결되었습니다.");
    }

    void onDisconnect(BLEServer* pServer) {
        Serial.println("BLE 클라이언트가 연결이 끊어졌습니다.");
        pServer->getAdvertising()->start();
        Serial.println("BLE 광고가 다시 시작되었습니다.");
    }
};

class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        bleCommand = pCharacteristic->getValue().c_str();
        bleCommand.trim();
        Serial.print("BLE 명령어 수신: ");
        Serial.println(bleCommand);

        if (bleCommand == "START_ALARM") {
            Serial.println("알람 시작 명령 수신!");
            startAlarmFlag = true;
        } else if (bleCommand.startsWith("SET_ALARM")) {
            int hour = bleCommand.substring(10, 12).toInt();
            int minute = bleCommand.substring(13, 15).toInt();
            setAlarmTime(hour, minute);
            Serial.print("알람이 ");
            Serial.print(hour);
            Serial.print("시 ");
            Serial.print(minute);
            Serial.println("분으로 설정되었습니다.");
        }
    }
};

void setupBLE() {
    BLEDevice::init("ESP32_Alarm_Control");
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService("12345678-1234-1234-1234-123456789abc");

    pCharacteristic = pService->createCharacteristic(
                        "87654321-4321-4321-4321-cba987654321",
                        BLECharacteristic::PROPERTY_WRITE
                      );

    pCharacteristic->setCallbacks(new MyCallbacks());
    pService->start();
    pServer->getAdvertising()->start();
    Serial.println("BLE 서버가 시작되었습니다.");
}
