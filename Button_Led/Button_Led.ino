 // ESP32에서는 GPIO 2가 내장 LED에 연결되어 있음
 const int ledPin = 15;
 int buttonPin = 4;
#include <Adafruit_NeoPixel.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  pinMode(15, OUTPUT);

  SerialBT.begin("ESP32_Test"); // Bluetooth 장치 이름 설정

  Serial.println("Bluetooth is ready. Waiting for input...");

}
void loop() {
  if (SerialBT.available()) { // Bluetooth 입력 데이터가 있으면
    String input = SerialBT.readStringUntil('\n'); // 입력된 문자열 읽기
    input.trim(); // 앞뒤 공백 제거

        // 숫자인지 확인 후 시리얼 모니터에 출력
    if (input.toInt() != 0 || input == "0") {
      Serial.println("Received number: " + input);
      SerialBT.println("Echo: " + input); // Bluetooth로 다시 응답 전송 (옵션)
    } else {
        Serial.println("Invalid input (not a number): " + input);
      }
  }

  digitalWrite(15, HIGH);
  delay(250);
  digitalWrite(15, LOW);
  delay(250);
}
