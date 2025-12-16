#pragma once

class TemperatureSensor {
 public:
  virtual float getTemperature() = 0;
  virtual ~TemperatureSensor() = default;
};
