#pragma once

#include "core/pins.hpp"
#include "core/scheduler.hpp"
#include "io/servo.hpp"
#include <Servo.h>
#include <stdint.h>

class ArduinoServoMotor : public ServoMotor, public ExternalInput {
private:
  Servo servo;
  int angle;

protected:
  void read() override;

public:
  ArduinoServoMotor(uint8_t pin, int initialAngle, int min, int max);
  int getAngle() override;
  void setAngle(int angle) override;
};
