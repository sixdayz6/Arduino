#include "bluetooth.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

BLEServer *bleServer = nullptr;
BLEAdvertising *bleAdvertising = nullptr; // 변수명 수정
BLECharacteristic *txCharacteristic = nullptr;

#define SERVICE_UUID           "12345678-1234-1234-1234-123456789abc"
#define CHARACTERISTIC_UUID_TX "abcd1234-1234-1234-1234-123456789abc"

void initBLE() {
    BLEDevice::init("ESP32_Alarm");
    bleServer = BLEDevice::createServer();

    // BLE Service 생성
    BLEService *service = bleServer->createService(SERVICE_UUID);

    // BLE Characteristic 생성
    txCharacteristic = service->createCharacteristic(
        CHARACTERISTIC_UUID_TX,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );

    service->start();

    // BLE Advertising 초기화
    bleAdvertising = BLEDevice::getAdvertising(); // BLEAdvertising 객체 생성
    bleAdvertising->addServiceUUID(SERVICE_UUID);
    bleAdvertising->setScanResponse(false);
    bleAdvertising->setMinPreferred(0x06); // 0x06 == 중간 광고 간격
    bleAdvertising->setMinPreferred(0x12); // 0x12 == 빠른 광고 간격

    Serial.println("BLE Initialized.");
}

void notifyBLE(String message) {
    if (txCharacteristic != nullptr) {
        txCharacteristic->setValue(message.c_str());
        txCharacteristic->notify();
        Serial.printf("BLE Notification: %s\n", message.c_str());
    }
}

void startBLEAdvertising() {
    if (bleAdvertising != nullptr) { // bleAdvertising으로 변경
        bleAdvertising->start();
        Serial.println("BLE Advertising started.");
    } else {
        Serial.println("BLE Advertising not initialized.");
    }
}
