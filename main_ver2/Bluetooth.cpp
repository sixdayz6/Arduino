#include "Bluetooth.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

BLEServer *pServer;
BLECharacteristic *pCharacteristic;

void initBluetooth() {
    BLEDevice::init("ESP32 Alarm");
    pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(BLEUUID("12345678-1234-1234-1234-123456789012"));
    pCharacteristic = pService->createCharacteristic(
        BLEUUID("87654321-4321-4321-4321-210987654321"),
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
    );
    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->start();
}

void processBluetooth() {
    // BLE 데이터 처리
}

void sendAlarmTriggered() {
    pCharacteristic->setValue("Alarm Actived");
    pCharacteristic->notify();
}
