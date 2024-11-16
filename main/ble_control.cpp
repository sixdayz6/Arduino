#include "ble_control.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "rtc_control.h"
#include "alarm_control.h"  // 알람 제어를 위해 포함

BLECharacteristic *pCharacteristic;
String bleCommand = "";       // BLE 명령어 저장
bool startAlarmFlag = false;  // 알람 시작 플래그

// BLE 서버 콜백 클래스
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    Serial.println("BLE 클라이언트가 연결되었습니다.");
  }

  void onDisconnect(BLEServer *pServer) {
    Serial.println("BLE 클라이언트가 연결이 끊어졌습니다.");
    pServer->getAdvertising()->start();
    Serial.println("BLE 광고가 다시 시작되었습니다.");
  }
};

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    bleCommand = pCharacteristic->getValue().c_str();  // 입력 값 저장
    bleCommand.trim();                                 // 문자열의 앞뒤 공백 제거
    Serial.print("BLE 명령어 수신: ");
    Serial.println(bleCommand);

    // 알람 시간 설정 명령 처리
    if (bleCommand.startsWith("SET_ALARM")) {
      int hour = bleCommand.substring(10, 12).toInt();
      int minute = bleCommand.substring(13, 15).toInt();
      setAlarmTime(hour, minute);
      Serial.print("알람이 ");
      Serial.print(hour);
      Serial.print("시 ");
      Serial.print(minute);
      Serial.println("분으로 설정되었습니다.");
    }
    // 즉시 알람 시작 명령 처리
    else if (bleCommand == "START_ALARM") {
      Serial.print("알람 시작 명령 수신!");
      startAlarmFlag = true;  // 플래그 설정
    }
  }
};

void setupBLE() {
  BLEDevice::init("ESP32_Alarm_Control");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(BLEUUID("12345678-1234-5678-1234-56789abcdef0"));

  pCharacteristic = pService->createCharacteristic(
    BLEUUID("abcdef12-3456-7890-abcd-ef1234567890"),
    BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  // pServer->getAdvertising()->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BLEUUID("abcdef12-3456-7890-abcd-ef1234567890"));
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("BLE 서버가 시작되었습니다. 이제 장치를 검색할 수 있습니다.");
}
