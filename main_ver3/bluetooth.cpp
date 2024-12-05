#include "Bluetooth.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Preferences.h>
#include <base64.h> // Include the Base64 library
#include "Alarm.h"
#include "rtc.h"
#include "config.h"

// UUIDs for BLE Service and Characteristics
#define SERVICE_UUID "12345678-1234-1234-1234-123456789012"
#define CHARACTERISTIC_UUID_RX "87654321-4321-4321-4321-210987654321" // Receive data
#define CHARACTERISTIC_UUID_TX "87654321-4321-4321-4321-210987654322" // Transmit data

BLEServer *pServer = nullptr;
BLECharacteristic *pTxCharacteristic = nullptr; // TX (ESP32 -> Client)
BLECharacteristic *pRxCharacteristic = nullptr; // RX (Client -> ESP32)
BLEAdvertising *pAdvertising = nullptr;

Preferences preferences;

bool deviceConnected = false;
std::string previousDeviceAddress = ""; // Store the last connected device's MAC address
unsigned long advertisingStartTime = 0;

// Save last device address to persistent storage
void saveLastDeviceAddress(const std::string &address) {
    preferences.begin("BLEPrefs", false);
    preferences.putString("lastDevice", address.c_str());
    preferences.end();
}

// Load last device address from persistent storage
std::string loadLastDeviceAddress() {
    preferences.begin("BLEPrefs", true);
    std::string address = preferences.getString("lastDevice", "").c_str();
    preferences.end();
    return address;
}

// Callback for connection events
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer, esp_ble_gatts_cb_param_t *param) {
        // Retrieve the MAC address of the connecting device
        char currentDeviceAddress[18];
        sprintf(currentDeviceAddress, "%02X:%02X:%02X:%02X:%02X:%02X",
                param->connect.remote_bda[0], param->connect.remote_bda[1], param->connect.remote_bda[2],
                param->connect.remote_bda[3], param->connect.remote_bda[4], param->connect.remote_bda[5]);

        Serial.println("Device connected: " + String(currentDeviceAddress));

        // Save the MAC address if it's a new connection
        if (previousDeviceAddress.empty() || previousDeviceAddress != currentDeviceAddress) {
            previousDeviceAddress = currentDeviceAddress;
            saveLastDeviceAddress(currentDeviceAddress); // Save to persistent storage
            Serial.println("Saved device address: " + String(currentDeviceAddress));
        }

        deviceConnected = true;
        bleSearching = false;
    }

    void onDisconnect(BLEServer *pServer) {
        Serial.println("Device disconnected");
        deviceConnected = false;
        startBLEAdvertising(); // Restart advertising for reconnection
    }
};

// Callback for receiving data
class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        String receivedData = pCharacteristic->getValue(); // Read the received data
        // receivedData = decodeBase64Data(pCharacteristic->getValue());
        Serial.println("Received data: " + String(receivedData.c_str()));

        // Handle mixed data types: integers and strings
        if (!receivedData.isEmpty()) {
            bool isInteger = true;
            for (char c : receivedData) {
                if (!isdigit(c)) {
                    isInteger = false;
                    break;
                }
            }

            if (isInteger) {
                // Handle integer input (e.g., alarm interval)
                int alarmInterval = atoi(receivedData.c_str());
                if (alarmInterval >= MIN_ALARM_INTERVAL && alarmInterval <= MAX_ALARM_INTERVAL) {
                    currentAlarm.interval = alarmInterval;
                    startCountdown();
                    Serial.printf("Alarm interval updated to %d minutes.\n", alarmInterval);
                    notifyBLE("Alarm interval set to " + String(alarmInterval) + " minutes.");
                } else {
                    Serial.println("Invalid interval. Must be between 30 and 1440 minutes.");
                    notifyBLE("Invalid interval. Must be between 30 and 1440 minutes.");
                }
            } else {
                // Handle string input
                String receivedString = String(receivedData.c_str());
                Serial.println("Received string: " + receivedString);
                notifyBLE("Received string: " + receivedString);
            }
        }
    }
};

// Advertising Control
void startBLEAdvertising() {
    Serial.println("Starting BLE Advertising...");
    pAdvertising->start();
    advertisingStartTime = millis();
}

void stopBLEAdvertising() {
    Serial.println("Stopping BLE Advertising...");
    pAdvertising->stop();
}

// Reconnect to the previously connected device
void reconnectToPreviousDevice() {
    if (!previousDeviceAddress.empty()) {
        Serial.println("Attempting to reconnect to previous device: " + String(previousDeviceAddress.c_str()));

        // Start advertising for reconnection
        startBLEAdvertising();
        unsigned long startTime = millis();

        // Wait for 1 minute to reconnect
        while (!deviceConnected && millis() - startTime < 60000) {
            delay(100);
        }

        if (deviceConnected) {
            Serial.println("Reconnected to previous device!");
        } else {
            Serial.println("Reconnection timeout. Stopping advertising.");
            stopBLEAdvertising();
        }
    } else {
        Serial.println("No previous device to reconnect to. Starting normal advertising.");
        startBLEAdvertising();
    }
}

// Initialize BLE
void initBluetooth() {
    Serial.println("Initializing Bluetooth...");
    BLEDevice::init("ESP32 BLE Server");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create TX Characteristic
    pTxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_TX,
        BLECharacteristic::PROPERTY_NOTIFY
    );

    // Create RX Characteristic
    pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        BLECharacteristic::PROPERTY_WRITE
    );

    pRxCharacteristic->setCallbacks(new MyCallbacks());
    pService->start();

    // Set up advertising
    pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);

    // Attempt to reconnect to the previous device
    previousDeviceAddress = loadLastDeviceAddress(); // Load the last connected device address
    reconnectToPreviousDevice();
}

// Notify connected BLE device
void notifyBLE(String message) {
    if (pTxCharacteristic != nullptr && deviceConnected) {
        pTxCharacteristic->setValue(message.c_str());
        pTxCharacteristic->notify();
        Serial.printf("BLE Notification: %s\n", message.c_str());
    }
}

// void decodeBase64Data(const String& base64Input) {
//     // Buffer to store decoded data
//     size_t decodedLen = base64_dec_len(base64Input.c_str(), base64Input.length());
//     char decodedData[decodedLen]; // Allocate memory for decoded output

//     // Decode Base64 input
//     base64_decode(decodedData, base64Input.c_str(), base64Input.length());

//     // Output the raw data
//     Serial.println("Decoded Data (Raw):");
//     for (size_t i = 0; i < decodedLen; i++) {
//         Serial.printf("0x%02X ", (uint8_t)decodedData[i]);
//     }
//     Serial.println();

//     // If decoded data is printable, output as a string
//     if (isPrintable(decodedData, decodedLen)) {
//         Serial.println("Decoded String:");
//         Serial.println(decodedData);
//     }
// }

// // Helper function to check if data is printable
// bool isPrintable(const char* data, size_t length) {
//     for (size_t i = 0; i < length; i++) {
//         if (!isprint(data[i])) {
//             return false;
//         }
//     }
//     return true;
// }

