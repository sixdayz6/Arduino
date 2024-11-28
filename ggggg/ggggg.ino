const int buttonPin1 = 13;  // 첫 번째 버튼 GPIO 핀
const int buttonPin2 = 12;  // 두 번째 버튼 GPIO 핀
const int ledPin = 14;      // 알람 시 켜질 LED 핀
const int ledPin1 = 15;     // 첫 번째 버튼의 LED 핀
const int ledPin2 = 16;     // 두 번째 버튼의 LED 핀

int lastButtonState1 = HIGH;  // 첫 번째 버튼의 이전 상태 저장
int lastButtonState2 = HIGH;  // 두 번째 버튼의 이전 상태 저장
unsigned long buttonPressTime1 = 0;
unsigned long buttonPressTime2 = 0;
int alarmTime = 0;            // 알람 시간 (단위: 분)
unsigned long startTime = 0;
bool alarmSet = false;        // 알람이 설정되었는지 여부
bool alarmActive = false;     // 알람이 울리고 있는지 여부
bool alarmLedOn = false;      // 알람 LED 상태

void setup() {
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);    // 알람 LED 핀 설정
  pinMode(ledPin1, OUTPUT);   // 첫 번째 버튼 LED 핀 설정
  pinMode(ledPin2, OUTPUT);   // 두 번째 버튼 LED 핀 설정
  Serial.begin(115200);
}

void loop() {
  int buttonState1 = digitalRead(buttonPin1);
  int buttonState2 = digitalRead(buttonPin2);

  // 첫 번째 버튼 짧은/긴 누름 감지
  if (buttonState1 == LOW) {
    if (lastButtonState1 == HIGH) {
      buttonPressTime1 = millis();
    } else if (millis() - buttonPressTime1 > 2000) {
      if (!alarmActive) {  // 알람이 울리지 않을 때만 카운트다운 시작
        Serial.println("카운트다운 시작");
        startTime = millis();
        alarmSet = true;

        // 첫 번째 LED 두 번 깜빡임
        for (int i = 0; i < 2; i++) {
          digitalWrite(ledPin1, HIGH);
          delay(200);
          digitalWrite(ledPin1, LOW);
          delay(200);
        }
      }
    }
  } else if (lastButtonState1 == LOW) {
    if (millis() - buttonPressTime1 <= 2000) {
      if (alarmLedOn) {
        // 알람이 울릴 때 첫 번째 버튼을 짧게 눌러 LED 끄기
        Serial.println("알람 종료 및 LED 끄기");
        digitalWrite(ledPin, LOW);  // 알람 LED 끄기
        alarmLedOn = false;         // LED 상태 변경
        alarmActive = false;        // 알람 종료
      } else {
        // 알람이 울리지 않는 경우: 1분 증가
        alarmTime += 1;
        Serial.print("1분 증가: ");
        Serial.print(alarmTime);
        Serial.println("분");

        // 첫 번째 버튼 LED 한 번 깜빡임
        digitalWrite(ledPin1, HIGH);
        delay(500);
        digitalWrite(ledPin1, LOW);
      }
    }
  }

  // 두 번째 버튼 짧은/긴 누름 감지
  if (buttonState2 == LOW) {
    if (lastButtonState2 == HIGH) {
      buttonPressTime2 = millis();
    } else if (millis() - buttonPressTime2 > 2000) {
      // 알람 초기화
      alarmTime = 0;
      alarmSet = false;
      alarmActive = false;
      alarmLedOn = false;
      Serial.println("알람 초기화");
      digitalWrite(ledPin, LOW);  // 알람 초기화 시 LED 끄기

      // 두 번째 LED 두 번 깜빡임
      for (int i = 0; i < 2; i++) {
        digitalWrite(ledPin2, HIGH);
        delay(200);
        digitalWrite(ledPin2, LOW);
        delay(200);
      }
    }
  } else if (lastButtonState2 == LOW) {
    if (millis() - buttonPressTime2 <= 2000) {
      // 알람 시간 1분 감소
      alarmTime -= 1;
      if (alarmTime < 0) alarmTime = 0;
      Serial.print("1분 감소: ");
      Serial.print(alarmTime);
      Serial.println("분");

      // 두 번째 버튼 LED 한 번 깜빡임
      digitalWrite(ledPin2, HIGH);
      delay(500);
      digitalWrite(ledPin2, LOW);
    }
  }

  // 알람 시간이 되었는지 확인
  if (alarmSet && (millis() - startTime >= alarmTime * 60000)) {
    if (!alarmLedOn) {
      Serial.println("알람 시간 도달!");
      digitalWrite(ledPin, HIGH);  // 알람 LED 켜기
      alarmLedOn = true;           // LED 상태 업데이트
      alarmActive = true;          // 알람 활성화
    }
  }

  // 현재 버튼 상태를 이전 상태 변수에 저장
  lastButtonState1 = buttonState1;
  lastButtonState2 = buttonState2;

  delay(10);  // 버튼 상태 변화를 확인하기 위한 짧은 지연
}
