#include "Switch.h"

Switch::Switch(int pin) : sensorPin(pin) {
    pinMode(sensorPin, INPUT);
}

bool Switch::isHigh() const {
    return digitalRead(sensorPin) == HIGH;
}

int Switch::getRawValue() const {
    return digitalRead(sensorPin);
}
