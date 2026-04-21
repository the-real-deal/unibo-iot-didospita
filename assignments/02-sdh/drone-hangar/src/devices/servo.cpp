#include "servo.hpp"

#include <Arduino.h>

ArduinoServoMotor::ArduinoServoMotor(Servo servo, int initialAngle)
    : servo(servo), angle(initialAngle) {}

void ArduinoServoMotor::setup()
{
  this->setAngle(this->angle);
}

void ArduinoServoMotor::read()
{
  this->angle = this->servo.read();
}

int ArduinoServoMotor::getAngle() { return this->angle; }

void ArduinoServoMotor::setAngle(int angle) { this->servo.write(angle); }
