#include "Bluetooth.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "Alarm.h"
#include "rtc.h"
#include "config.h"
#include "output.h"

// UUIDs for BLE Service and Characteristics
#define SERVICE_UUID "12345678-1234-1234-1234-123456789012"
#define CHARACTERISTIC_UUID_RX "87654321-4321-4321-4321-210987654321" // Receive data
#define CHARACTERISTIC_UUID_TX "87654321-4321-4321-4321-210987654322" // Transmit data

BLEServer *pServer = nullptr;
BLECharacteristic *pTxCharacteristic = nullptr; // TX characteristic (ESP32 -> App)
BLECharacteristic *pRxCharacteristic = nullptr; // RX characteristic (App -> ESP32)
BLEAdvertising *pAdvertising = nullptr;

bool deviceConnected = false;

// Callback for connection events
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
        deviceConnected = true;
        Serial.println("Device connected");
    }

    void onDisconnect(BLEServer *pServer) {
        deviceConnected = false;
        Serial.println("Device disconnected");
        startBLEAdvertising(); // Restart advertising after disconnection
    }
};

// Callback for receiving data
class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        String receivedData = pCharacteristic->getValue();
        if (!receivedData.isEmpty()) {
            String command = String(receivedData.c_str()); // Convert to Arduino String
            Serial.println("Received data via BLE: " + command);

            // Handle received commands
            if (command == "SET_ALARM") {
                currentAlarm.interval = 30; // Example: Set alarm for 30 minutes
                startCountdown();
                notifyBLE("Alarm set for 30 minutes");
            } else if (command == "GET_ALARM") {
                String message = "Next alarm in " + String(currentAlarm.interval) + " minutes.";
                notifyBLE(message);
            } else if (command == "CLEAR_ALARM") {
                resetAlarms();
                notifyBLE("All alarms cleared");
            } else {
                notifyBLE("Unknown command");
            }

            // 수신 데이터 구문 분석
        int separatorIndex = command.indexOf(':');
        if (separatorIndex == -1) {
            Serial.println("Invalid command format.");
            return;
        }

        // 명령어와 값 분리
        String cmd = command.substring(0, separatorIndex);
        String arg = command.substring(separatorIndex + 1);

        // 명령어 처리
        if (cmd == "SET_INTERVAL") {
            int alarmInterval = arg.toInt();
            if (alarmInterval >= 30 && alarmInterval <= 1440) {
                currentAlarm.interval = alarmInterval;
                startCountdown();
                Serial.printf("Alarm interval updated to %d minutes.\n", alarmInterval);
            } else {
                Serial.println("Invalid interval. Must be between 30 and 1440 minutes.");
            }
        } else if (cmd == "SET_TONE") {
            int toneFrequency = arg.toInt();
            if (toneFrequency > 0) {
                setSpeakerTone(toneFrequency);
                Serial.printf("Speaker tone set to %d Hz.\n", toneFrequency);
            } else {
                Serial.println("Invalid tone frequency.");
            }
        } else {
            Serial.println("Unknown command.");
        }


        }
    }
};

// Initialize BLE
void initBluetooth() {
    BLEDevice::init("ESP32 Alarm");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create TX (Notify) Characteristic
    pTxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_TX,
        BLECharacteristic::PROPERTY_NOTIFY
    );

    // Create RX (Write) Characteristic
    pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        BLECharacteristic::PROPERTY_WRITE
    );

    // Set RX Callback
    pRxCharacteristic->setCallbacks(new MyCallbacks());

    // Start the service
    pService->start();

    // Start advertising
    pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();
    Serial.println("BLE initialized and advertising");
    String btMacAddress = BLEDevice::getAddress().toString().c_str();
    Serial.println("Bluetooth MAC Address:");
    Serial.println(btMacAddress);
}

// Process incoming BLE commands
void processBluetooth() {
    if (!deviceConnected) return;
    // Additional logic if needed
}

// Notify connected BLE device
void notifyBLE(String message) {
    if (pTxCharacteristic != nullptr && deviceConnected) {
        pTxCharacteristic->setValue(message.c_str());
        pTxCharacteristic->notify();
        Serial.printf("BLE Notification: %s\n", message.c_str());
    }
}

// Restart BLE advertising
void startBLEAdvertising() {
    if (pAdvertising != nullptr) {
        pAdvertising->start();
        Serial.println("BLE Advertising started.");
    } else {
        Serial.println("BLE Advertising not initialized.");
    }
}
