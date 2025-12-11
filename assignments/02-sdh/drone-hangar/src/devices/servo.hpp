#pragma once

#include "core/pins.hpp"
#include <Servo.h>
#include <stdint.h>

class ServoMotor {
private:
  DigitalOutputPin pin;
  Servo servo;
  int angle;

public:
  ServoMotor(uint8_t pin, int initialAngle, int min, int max);
  int getAngle();
  void setAngle(int angle);
};
