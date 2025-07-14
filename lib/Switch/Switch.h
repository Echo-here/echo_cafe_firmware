#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include <Arduino.h>

class Switch {
public:
    // 생성자
    Switch(int pin);

    // 현재 조도 상태 반환 (true: 밝음, false: 어두움)
    bool isHigh() const;

    // 디지털 값 직접 반환 (HIGH or LOW)
    int getRawValue() const;

private:
    int sensorPin;
};

#endif // LIGHTSENSOR_H
