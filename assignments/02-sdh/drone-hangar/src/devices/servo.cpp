#include "servo.hpp"

#include <Arduino.h>

#include "config.h"

ArduinoServoMotor::ArduinoServoMotor(uint8_t pin,int initialAngle)
    : servo(),
      pin(pin),
      angle(initialAngle) {}

void ArduinoServoMotor::setup()
{
  this->servo.attach(this->pin, SERVO_MIN_FREQ, SERVO_MAX_FREQ);
  while (!this->servo.attached())
    ;
  this->setAngle(this->angle);
}

void ArduinoServoMotor::read()
{
  this->angle = this->servo.read();
}

int ArduinoServoMotor::getAngle() { return this->angle; }

void ArduinoServoMotor::setAngle(int angle) { this->servo.write(angle); }
