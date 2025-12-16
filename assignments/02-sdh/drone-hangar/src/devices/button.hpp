#pragma once

#include "core/pins.hpp"
#include "core/scheduler.hpp"
#include "io/button.hpp"

class PushButton : public Button, public ExternalInput {
  DigitalInputPin pin;
  bool pressed;

public:
  PushButton(uint8_t pin);
  void read() override;
  bool isPressed() override;
};
