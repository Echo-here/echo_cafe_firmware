#ifndef SERVOMT_H
#define SERVOMT_H

#include <Arduino.h>
#include <Servo.h>

// Define 최소, 최대 각도
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 90

class ServoMT {
public:
    // 생성자
    ServoMT(int pin);

    // 서보 각도 설정
    void setAngle(int angle);
    // 서보 최대 각도 설정
    void setMaxAngle();
    // 서보 최소 각도 설정
    void setMinAngle();
    // 현재 각도 반환
    int getAngle() const;

private:
    int servoPin;
    int currentAngle;
    Servo servo;
};

#endif // SERVOMT_H
