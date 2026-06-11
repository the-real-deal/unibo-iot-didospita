#include "servo.hpp"

#include <Arduino.h>

ServoMotor::ServoMotor(uint8_t pin, int initialAngle)
    : servo(),
      pin(pin),
      angle(initialAngle) {}

void ServoMotor::setup()
{
  this->servo.attach(this->pin, SERVO_MIN_FREQ, SERVO_MAX_FREQ);
  while (!this->servo.attached())
    ;
  this->setAngle(this->angle);
}

void ServoMotor::read()
{
  noInterrupts();
  this->angle = this->servo.read();
  interrupts();
}

int ServoMotor::getAngle() { return this->angle; }

void ServoMotor::setAngle(int angle) { this->servo.write(angle); }
