#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
bool deviceConnected = false;

// BLE 서버 콜백 클래스 정의
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      // 연결이 끊기면 다시 광고 시작
      pServer->startAdvertising();
      Serial.println("Advertising restarted for reconnection");
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  // BLE 초기화
  BLEDevice::init("ESP32 BLE");

  // BLE 서버 생성
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // BLE 서비스 생성
  BLEService *pService = pServer->createService(BLEUUID("12345678-1234-5678-1234-56789abcdef0"));

  // BLE 특성 추가
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         BLEUUID("abcdef12-3456-7890-abcd-ef1234567890"),
                                         BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
                                       );
  pCharacteristic->setValue("Hello World");
  
  // 서비스 시작
  pService->start();

  // 광고 시작
  // pServer->getAdvertising()->start();
  // Serial.println("Waiting for a client connection...");

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BLEUUID("12345678-1234-5678-1234-56789abcdef0"));
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  // 연결 상태 출력
  if (deviceConnected) {
    Serial.println("Device connected");
  } else {
    Serial.println("Device disconnected");
  }

  delay(2000);  // 상태 확인을 위해 2초마다 출력
}
