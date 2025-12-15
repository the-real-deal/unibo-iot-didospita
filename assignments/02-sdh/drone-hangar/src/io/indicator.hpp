#pragma once

class Indicator {
public:
  virtual void turnOn() = 0;
  virtual void turnOff() = 0;
  virtual bool isOn() = 0;
  virtual void toggle() {
    if (this->isOn()) {
      this->turnOff();
    } else {
      this->turnOff();
    }
  };
  virtual ~Indicator() = default;
};
