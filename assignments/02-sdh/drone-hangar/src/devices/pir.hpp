#pragma once

#include "core/pins.hpp"
#include "core/sensors.hpp"

class PIR : public Sensor<DigitalValue> {
private:
  DigitalInputPin pin;

public:
  PIR(uint8_t pin);
  void read() override;
};
