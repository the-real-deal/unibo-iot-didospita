#include "servo.hpp"

#include <Arduino.h>

ArduinoServoMotor::ArduinoServoMotor(uint8_t pin, int initialAngle, int min,
                                     int max)
    : servo(), angle(initialAngle) {
  this->servo.attach(pin, min, max);
  while (!this->servo.attached());
  this->setAngle(initialAngle);
}

void ArduinoServoMotor::read() { this->angle = this->servo.read(); }

int ArduinoServoMotor::getAngle() { return this->angle; }

void ArduinoServoMotor::setAngle(int angle) { this->servo.write(angle); }
