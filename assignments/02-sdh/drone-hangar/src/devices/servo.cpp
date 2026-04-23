#include "servo.hpp"

#include <Arduino.h>

ArduinoServoMotor::ArduinoServoMotor(uint8_t pin,
                                     int minFreq,
                                     int maxFreq,
                                     int initialAngle)
    : servo(),
      pin(pin),
      minFreq(minFreq),
      maxFreq(maxFreq),
      angle(initialAngle) {}

void ArduinoServoMotor::setup()
{
  this->servo.attach(this->pin, this->minFreq, this->maxFreq);
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
