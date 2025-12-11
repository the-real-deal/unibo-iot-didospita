#include "servo.hpp"
#include <Arduino.h>

ServoMotor::ServoMotor(uint8_t pin, int initialAngle, int min, int max)
    : pin(pin), servo(), angle(initialAngle) {
  this->servo.attach(pin, min, max);
  while (!this->servo.attached())
    ;
}

int ServoMotor::getAngle() { return this->angle; }

void ServoMotor::setAngle(int angle) {
  Serial.print("Servo: ");
  Serial.println(angle);
  this->angle = angle;
  this->servo.write(angle);
}
