#pragma once

#include <Servo.h>
#include <stdint.h>

class ServoMotor {
private:
  Servo servo;

public:
  ServoMotor(uint8_t pin);
  ServoMotor(uint8_t pin, int min, int max);
  void setAngle(int angle);
};
