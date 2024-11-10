#define BUTTON1_PIN 12 // 1번 버튼 핀 번호
#define BUTTON2_PIN 13 // 2번 버튼 핀 번호

unsigned long lastPressTime1 = 0;
unsigned long lastPressTime2 = 0;
unsigned long pressDuration1 = 0;
unsigned long pressDuration2 = 0;
int alarmTime = 0; // 알람 시간 (분 단위)
const int MAX_ALARM = 540; // 9시간 = 540분
const int ALARM_STEP = 30; // 30분 단위 증가/감소

void setup() {
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  bool button1State = !digitalRead(BUTTON1_PIN);
  bool button2State = !digitalRead(BUTTON2_PIN);

  if (button1State) {
    if (millis() - lastPressTime1 > 1000 && lastPressTime1 != 0) {
      pressDuration1 = millis() - lastPressTime1;
    }
    lastPressTime1 = millis();
  } else if (pressDuration1 > 0) {
    handleButton1(pressDuration1);
    pressDuration1 = 0;
  }

  if (button2State) {
    if (millis() - lastPressTime2 > 1000 && lastPressTime2 != 0) {
      pressDuration2 = millis() - lastPressTime2;
    }
    lastPressTime2 = millis();
  } else if (pressDuration2 > 0) {
    handleButton2(pressDuration2);
    pressDuration2 = 0;
  }
}

// 버튼 1의 기능: 시간 증가 및 초기화
void handleButton1(unsigned long duration) {
  if (duration < 1000) { // 짧게 누름 (30분 증가)
    if (alarmTime + ALARM_STEP <= MAX_ALARM) {
      alarmTime += ALARM_STEP;
      Serial.print("알람 시간 설정: ");
      Serial.print(alarmTime / 60);
      Serial.print("시간 ");
      Serial.print(alarmTime % 60);
      Serial.println("분");
    } else {
      Serial.println("알람 시간은 최대 9시간으로 제한됩니다.");
    }
  } else if (duration >= 3000) { // 길게 누름 (알람 초기화)
    alarmTime = 0;
    Serial.println("알람 시간이 초기화되었습니다.");
  }
}

// 버튼 2의 기능: 시간 감소 및 알람 삭제
void handleButton2(unsigned long duration) {
  if (duration < 1000) { // 짧게 누름 (30분 감소)
    if (alarmTime - ALARM_STEP >= 0) {
      alarmTime -= ALARM_STEP;
      Serial.print("알람 시간 설정: ");
      Serial.print(alarmTime / 60);
      Serial.print("시간 ");
      Serial.print(alarmTime % 60);
      Serial.println("분");
    } else {
      Serial.println("알람 시간은 0분 이하로 설정할 수 없습니다.");
    }
  } else if (duration >= 3000) { // 길게 누름 (알람 삭제)
    alarmTime = 0;
    Serial.println("알람이 삭제되었습니다.");
  }
}
