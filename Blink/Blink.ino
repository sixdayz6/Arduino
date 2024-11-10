 // ESP32에서는 GPIO 2가 내장 LED에 연결되어 있음

void setup() {
  // LED를 15번에 연결 했으니 출력모드로 하겠다.
  Serial.begin(115200);
}
void loop() {
  Serial.println("I'm Active");
  delay(1000);
}
