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
        // Get the received data
        String receivedData = pCharacteristic->getValue().c_str();

        if (!receivedData.isEmpty()) {
            Serial.println("Received data via BLE: " + receivedData);

            // Try to parse the input as an integer
            bool isInteger = true;
            for (size_t i = 0; i < receivedData.length(); i++) {
                if (!isdigit(receivedData[i])) {
                    isInteger = false;
                    break;
                }
            }

            if (isInteger) {
                // Handle integer input: Set alarm interval
                int alarmInterval = receivedData.toInt();
                if (alarmInterval >= 30 && alarmInterval <= 1440) {
                    currentAlarm.interval = alarmInterval;
                    startCountdown();
                    Serial.printf("Alarm interval updated to %d minutes.\n", alarmInterval);
                    notifyBLE("Alarm interval set to " + String(alarmInterval) + " minutes.");
                } else {
                    Serial.println("Invalid interval. Must be between 30 and 1440 minutes.");
                    notifyBLE("Invalid interval. Must be between 30 and 1440 minutes.");
                }
            } else {
                // Handle string input: Store the string in a characteristic and print it
                pCharacteristic->setValue(receivedData.c_str());
                Serial.println("Stored string in characteristic: " + receivedData);
                notifyBLE("Received string: " + receivedData);
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

    // Set advertising data compatible with iOS
    BLEAdvertisementData advertisementData;
    advertisementData.setFlags(0x06); // General discoverable mode, BR/EDR not supported
    advertisementData.setName("ESP32 Alarm");
    pAdvertising->setAdvertisementData(advertisementData);

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
