#pragma once

#include "core/pins.hpp"
#include "core/setup.hpp"

class Led : public Setup
{
private:
  DigitalOutputPin pin;
  DigitalValue value;
  void setValue(DigitalValue value);

public:
  Led(uint8_t pin);

  void setup() override;
  void turnOn();
  void turnOff();
  void toggle();
  bool isOn();
  bool isOff();
};
