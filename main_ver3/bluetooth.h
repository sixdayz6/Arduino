#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>

// 함수 선언
void initBluetooth();             // BLE 초기화
void notifyBLE(String message); // BLE 메시지 전송
void startBLEAdvertising(); // BLE 광고 시작
void processBluetooth(); // 블루투스 기능 실행

#endif // BLUETOOTH_H
