#include "ServoMT.h"

ServoMT::ServoMT(int pin, String name) : servoPin(pin), currentAngle(SERVO_MIN_ANGLE), name(name) {
    pinMode(servoPin, OUTPUT);
    servo.attach(servoPin);
    servo.write(currentAngle);  // 초기 각도 설정
    Serial.println(name + " ServoMT initialized");
}

void ServoMT::setAngle(int angle) {
    // 최소, 최대 범위 내로 제한
    if (angle < SERVO_MIN_ANGLE) angle = SERVO_MIN_ANGLE;
    if (angle > SERVO_MAX_ANGLE) angle = SERVO_MAX_ANGLE;

    servo.write(angle);
    currentAngle = angle;
}

void ServoMT::setMaxAngle() {
    setAngle(SERVO_MAX_ANGLE);
}

void ServoMT::setMinAngle() {
    setAngle(SERVO_MIN_ANGLE);
}

int ServoMT::getAngle() const {
    return currentAngle;
}

String ServoMT::getName() const {
    return name;
}
