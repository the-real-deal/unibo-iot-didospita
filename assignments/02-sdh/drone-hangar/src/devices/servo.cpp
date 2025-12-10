#include "servo.hpp"

ServoMotor::ServoMotor(uint8_t pin) : servo() { this->servo.attach(pin); }
ServoMotor::ServoMotor(uint8_t pin, int min, int max) : servo() {
  this->servo.attach(pin, min, max);
}

void ServoMotor::setAngle(int angle) { this->servo.write(angle); }
