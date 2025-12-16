#pragma once

#include "core/pins.hpp"
#include "io/indicator.hpp"

class Led : public Indicator {
 private:
  DigitalOutputPin pin;
  DigitalValue value;
  void setValue(DigitalValue value);

 public:
  Led(uint8_t pin);

  void turnOn() override;
  void turnOff() override;
  bool isOn() override;
  bool isOff() override;
};
