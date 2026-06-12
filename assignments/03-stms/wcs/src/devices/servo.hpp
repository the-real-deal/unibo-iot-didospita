#pragma once

#include <Servo.h>
#include <stdint.h>

#include "io.hpp"
#include "kernel/pins.hpp"
#include "kernel/events.hpp"

#ifndef SERVO_MIN_FREQ
#define SERVO_MIN_FREQ 500
#endif

#ifndef SERVO_MAX_FREQ
#define SERVO_MAX_FREQ 2500
#endif

class ServoMotor : public ExternalInput
{
private:
  Servo servo;
  uint8_t pin;
  int angle;

public:
  ServoMotor(uint8_t pin, int initialAngle);
  void setup() override;
  void read() override;
  int getAngle();
  void setAngle(int angle);
};
