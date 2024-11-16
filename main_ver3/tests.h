#ifndef TESTS_H
#define TESTS_H

#include <Arduino.h>

// 테스트 함수 선언
void testSpeaker();                     // 스피커 테스트
void testButtons();                     // 버튼 테스트
void testAlarmInterval();               // 알람 테스트
void testBluetooth();                   // 블루투스 연결 테스트

// 테스트 메뉴를 Serial Monitor에서 호출
void showTestMenu();
void handleTestCommand(String command);

#endif // TESTS_H
