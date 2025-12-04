#pragma once

#include "core/pins.hpp"
#include "core/sensors.hpp"

class TestSensor : public Sensor<double> {
private:
  AnalogInputPin pin;

public:
  TestSensor(uint8_t pin);
  void read() override;
};
