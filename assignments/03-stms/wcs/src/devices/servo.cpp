#include "servo.hpp"

#include <Arduino.h>

ServoMotor::ServoMotor(uint8_t pin, int initialAngle, EventFamily family)
    : EventSource(family),
      servo(),
      pin(pin),
      angle(initialAngle) {}

bool ServoMotor::writeAngle(int angle)
{
  this->servo.write(angle);
  return this->generateEvent({.angle = angle});
}

void ServoMotor::begin(EventsManager *eventsManager)
{
  EventSource<ServoMotorEvent>::begin(eventsManager);
  this->servo.attach(this->pin, SERVO_MIN_FREQ, SERVO_MAX_FREQ);
  while (!this->servo.attached())
    ;
  this->writeAngle(this->angle);
}

bool ServoMotor::setAngle(int angle)
{
  if (this->angle == angle)
  {
    return false;
  }
  this->angle = angle;
  return this->writeAngle(this->angle);
}
