#ifndef PUMPMT_H
#define PUMPMT_H

#include <Arduino.h>

// 펌프 상태 정의
#define PUMP_OFF LOW     // 펌프 정지
#define PUMP_ON HIGH     // 펌프 작동

class PumpMT {
public:
    // 생성자
    PumpMT(int pin, String name);

    // 펌프 제어
    void turnOn();
    void turnOff();
    void toggle();
    
    // 펌프 상태 확인
    bool isOn() const;
    bool isOff() const;
    
    // 펌프 작동 (지정된 시간 동안)
    void runForDuration(unsigned long durationMs);
    
    // 현재 상태 문자열 반환
    String getStateString();
    
    // 핀 번호 반환
    int getPin() const;
    
    // 이름 반환
    String getName() const;

private:
    int pumpPin;
    bool pumpState;
    String name;
};

#endif // PUMPMT_H 