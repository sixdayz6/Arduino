#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEAdvertising.h>
#include <esp_bt_device.h>
#include <Preferences.h> // NVS에 데이터를 저장하기 위해 사용

BLEServer* pServer = nullptr;
BLECharacteristic* pCharacteristic = nullptr;
bool deviceConnected = false;
bool allowPairingMode = false;

#define SERVICE_UUID           "12345678-1234-1234-1234-1234567890AB"
#define CHARACTERISTIC_UUID    "87654321-4321-4321-4321-BA0987654321"

#define BUTTON_PIN  12 // 버튼이 연결된 핀 번호 (예: GPIO 12)
#define BUTTON_HOLD_TIME 5000 // 버튼을 5초 이상 누를 때 (밀리초 단위)

// 버튼 관련 변수
unsigned long buttonPressStartTime = 0;
bool buttonPressed = false;

// Preferences 객체 생성 (암호화 키를 저장하기 위해 사용)
Preferences preferences;

// 보안 콜백 설정 (페어링 기능 관리)
class MySecurity : public BLESecurityCallbacks {
    uint32_t onPassKeyRequest() {
        Serial.println("Passkey Request");
        return 123456; // 고정된 패스키 사용
    }

    void onPassKeyNotify(uint32_t pass_key) {
        Serial.printf("Passkey Notify: %d\n", pass_key);
    }

    bool onConfirmPIN(uint32_t pass_key) {
        Serial.printf("Confirm Passkey: %d\n", pass_key);
        return true; // 패스키 확인
    }

    bool onSecurityRequest() {
        return true; // 보안 연결 요청 수락
    }

    void onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl) {
        if (cmpl.success) {
            Serial.println("Authentication Success");

            // 페어링이 성공했을 때, 암호화 키 저장 (Bonding)
            preferences.begin("ble", false);
            preferences.putString("bonded", "true");
            preferences.end();
            Serial.println("Bonding info saved");
        } else {
            Serial.println("Authentication Failed");
        }
    }
};

// BLE 서버 콜백 클래스 정의
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
        Serial.println("Device connected");
        pServer->getAdvertising()->stop(); // 연결 시 광고 중지
    }

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
        Serial.println("Device disconnected");
        if (!allowPairingMode) {
            // 기본적으로는 이전에 페어링된 장치와의 재연결만 허용
            pServer->startAdvertising();
            Serial.println("Start advertising for reconnection");
        }
    }
};

void setup() {
    Serial.begin(115200);

    // 버튼 핀 설정
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // BLE 초기화 및 설정
    BLEDevice::init("ESP32_BLE_Device");

    // BLE 보안 설정
    BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_MITM); // Man-In-The-Middle 보호
    BLEDevice::setSecurityCallbacks(new MySecurity()); // MySecurity 인스턴스를 설정

    // NVS에 저장된 bonding 정보 읽기
    preferences.begin("ble", false);
    String bonded = preferences.getString("bonded", "false");
    preferences.end();

    if (bonded == "true") {
        Serial.println("Bonded device found. Enabling auto-reconnect...");
    }

    // BLE 서버 생성
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // 서비스 생성
    BLEService* pService = pServer->createService(SERVICE_UUID);

    // 특성 생성 (Notify 사용)
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY
    );

    // 특성 값 설정
    pCharacteristic->setValue("Hello BLE");

    // 서비스 시작
    pService->start();

    // 광고 설정 (페어링된 장치만 허용)
    BLEAdvertising* pAdvertising = pServer->getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true); // 스캔 응답 포함
    pAdvertising->start();

    Serial.println("Waiting for a connection...");
}

void loop() {
    // 버튼 상태 확인
    int buttonState = digitalRead(BUTTON_PIN);

    if (buttonState == LOW) { // 버튼이 눌렸을 때
        if (!buttonPressed) {
            buttonPressStartTime = millis();
            buttonPressed = true;
        } else {
            // 버튼을 누르고 있는 시간 확인
            if (millis() - buttonPressStartTime >= BUTTON_HOLD_TIME) {
                // 버튼을 5초 이상 누름 -> 새로운 마스터와 페어링 허용
                Serial.println("Button held for 5 seconds, allowing new pairing...");
                allowPairingMode = true;
                pServer->getAdvertising()->start(); // 새로운 광고 시작
                buttonPressed = false; // 버튼 처리 완료 후 상태 초기화
            }
        }
    } else {
        // 버튼이 눌리지 않았을 때 초기화
        buttonPressed = false;
    }

    // 기타 루프 작업 처리
    delay(100);
}
