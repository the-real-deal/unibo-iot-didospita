#pragma once

#include "core/pins.hpp"
#include "io/indicator.hpp"

class Led : public Indicator, public Setup
{
private:
  DigitalOutputPin pin;
  DigitalValue value;
  void setValue(DigitalValue value);

public:
  Led(uint8_t pin);

  void setup() override;
  void turnOn() override;
  void turnOff() override;
  bool isOn() override;
  bool isOff() override;
};
