#pragma once

#include "core/pins.hpp"
#include "core/scheduler.hpp"
#include "io/button.hpp"

class PushButton : public Button, public ExternalInput
{
  DigitalInputPin pin;
  bool pressed;

protected:
  void setup() override;
  void read() override;

public:
  PushButton(uint8_t pin);
  bool isPressed() override;
};
