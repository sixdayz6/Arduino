//블루투스 라이브러리
#include <BluetoothSerial.h>

BluetoothSerial SerialBT; // Bluetooth 시리얼 객체 생성, setup, loop에서 모두 사용 가능한 변수


void setup() {
  Serial.begin(115200); // 통신속도 115200으로 설정
  SerialBT.begin("ESP32_Test"); // Bluetooth 장치 이름 설정

  Serial.println("Bluetooth is ready. Waiting for input..."); // 연결이 Serial 기능이 작동하는지 확인하기 위한 메세지


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

}
