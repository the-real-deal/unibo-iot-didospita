#pragma once

#include <Servo.h>
#include <stdint.h>

#include "core/pins.hpp"
#include "core/scheduler.hpp"
#include "io/servo.hpp"

class ArduinoServoMotor : public ServoMotor, public ExternalInput
{
private:
  Servo servo;
  int angle;

protected:
  void setup() override;
  void read() override;

public:
  ArduinoServoMotor(Servo servo, int initialAngle);
  int getAngle() override;
  void setAngle(int angle) override;
};
