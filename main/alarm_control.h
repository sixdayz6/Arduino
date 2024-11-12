#ifndef ALARM_CONTROL_H
#define ALARM_CONTROL_H

extern const int SPEAKER_PIN;

// 함수 선언
void setupSpeaker();
void playAlarm();
void playMelody();
void setupVibrationMotor();
void activateVibrationMotor();
void deactivateVibrationMotor();


#endif // ALARM_CONTROL_H
