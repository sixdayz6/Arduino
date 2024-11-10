#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

#define BUTTON1_PIN 12
#define BUTTON2_PIN 13

unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long debounceDelay = 50;
int button1State;
int button2State;

int alarmHours = 0;
int alarmMinutes = 0;
bool alarmSet = false;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON1_PIN, INPUT_PULLDOWN);
  pinMode(BUTTON2_PIN, INPUT_PULLDOWN);
  Wire.begin(21, 22);
  
  if (!rtc.begin()) {
    Serial.println("RTC 모듈이 연결되지 않았습니다!");
    while (1);
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // 초기 RTC 시간 설정
}

void loop() {

  delay(500);
  DateTime now = rtc.now();
  
  // 버튼 입력 확인
  button1State = digitalRead(BUTTON1_PIN);
  button2State = digitalRead(BUTTON2_PIN);
  
  // 버튼 1: 알람 시간 증가
  if (button1State == HIGH && millis() - lastDebounceTime1 > debounceDelay) {
    lastDebounceTime1 = millis();
    alarmMinutes += 30;
    if (alarmMinutes >= 60) {
      alarmMinutes = 0;
      alarmHours++;
    }
    if (alarmHours > 9) {  // 최대 9시간 설정
      alarmHours = 9;
      alarmMinutes = 0;
    }
    Serial.print("설정된 알람: ");
    Serial.print(alarmHours);
    Serial.print("시간 ");
    Serial.print(alarmMinutes);
    Serial.println("분");
  }
  
  // 버튼 2: 알람 시간 감소
  if (button2State == HIGH && millis() - lastDebounceTime2 > debounceDelay) {
    lastDebounceTime2 = millis();
    if (alarmMinutes == 0 && alarmHours == 0) {
      // 알람 최소 시간 (0:00)
      Serial.println("알람 시간이 이미 최소입니다.");
    } else {
      alarmMinutes -= 30;
      if (alarmMinutes < 0) {
        alarmMinutes = 30;
        if (alarmHours > 0) alarmHours--;
      }
      Serial.print("설정된 알람: ");
      Serial.print(alarmHours);
      Serial.print("시간 ");
      Serial.print(alarmMinutes);
      Serial.println("분");
    }
  }

  // 카운트다운 시작 (버튼 1 길게 누름)
  if (button1State == HIGH && millis() - lastDebounceTime1 > 1000 && millis() - lastDebounceTime1 < 3000) {
    Serial.println("카운트다운 시작");
    alarmSet = true;
  }

  // 현재 설정된 알람을 삭제 (버튼 1과 2 동시에 짧게 클릭)
  if (button1State == HIGH && button2State == HIGH && millis() - lastDebounceTime1 < 1000 && millis() - lastDebounceTime2 < 1000) {
    Serial.println("알람 삭제");
    alarmHours = 0;
    alarmMinutes = 0;
    alarmSet = false;
  }

  // 현재 시간과 알람 시간 비교
  if (alarmSet) {
    DateTime alarmTime = now + TimeSpan(0, alarmHours, alarmMinutes, 0);
    if (now >= alarmTime) {
      Serial.println("알람이 울립니다!");
      // 알람이 울릴 때 버튼 2를 길게 눌러 종료
      if (button2State == HIGH && millis() - lastDebounceTime2 > 1000 && millis() - lastDebounceTime2 < 3000) {
        Serial.println("알람 종료");
        alarmSet = false;
      }
    }
  }
}