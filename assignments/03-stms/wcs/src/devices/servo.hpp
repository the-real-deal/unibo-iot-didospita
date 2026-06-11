#pragma once

#include <Servo.h>
#include <stdint.h>

#include "core/setup.hpp"
#include "core/pins.hpp"

#ifndef SERVO_MIN_FREQ
#define SERVO_MIN_FREQ 500
#endif

#ifndef SERVO_MAX_FREQ
#define SERVO_MAX_FREQ 2500
#endif

class ArduinoServoMotor : public Setup
{
private:
  Servo servo;
  uint8_t pin;
  int angle;

protected:
  void setup() override;

public:
  ArduinoServoMotor(uint8_t pin, int initialAngle);
  int getAngle();
  void setAngle(int angle);
};
