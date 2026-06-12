#pragma once

#include <Servo.h>
#include <stdint.h>

#include "kernel/pins.hpp"
#include "kernel/events.hpp"

#ifndef SERVO_MIN_FREQ
#define SERVO_MIN_FREQ 500
#endif

#ifndef SERVO_MAX_FREQ
#define SERVO_MAX_FREQ 2500
#endif

struct ServoMotorEvent
{
  int angle;
};

class ServoMotor : public EventSource<ServoMotorEvent>
{
private:
  Servo servo;
  uint8_t pin;
  int angle;

  bool writeAngle(int angle);

public:
  ServoMotor(uint8_t pin, int initialAngle, EventFamily family);
  bool setAngle(int angle);
  void begin(EventsManager *eventsManager) override;
};
