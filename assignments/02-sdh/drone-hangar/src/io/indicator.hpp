#pragma once

class Indicator {
 public:
  virtual void turnOn() = 0;
  virtual void turnOff() = 0;
  virtual bool isOn() = 0;
  virtual bool isOff() = 0;  // just for readability

  virtual ~Indicator() = default;
};
