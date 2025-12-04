#pragma once

#include <Arduino.h>
#include <stdint.h>

enum class PinType : uint8_t {
  Input = INPUT,
  Output = OUTPUT,
};

class BasePin {
protected:
  uint8_t pin;
  PinType type;
  BasePin(uint8_t pin, PinType type);

public:
  uint8_t getPin();
  PinType getType();
};

enum class DigitalValue : uint8_t {
  High = HIGH,
  Low = LOW,
};

class DigitalOutputPin : public BasePin {
public:
  DigitalOutputPin(uint8_t pin);
  void write(DigitalValue status);
};

class DigitalInputPin : public BasePin {
public:
  DigitalInputPin(uint8_t pin);
  DigitalValue read();
};

class AnalogOutputPin : public BasePin {
public:
  AnalogOutputPin(uint8_t pin);
  void write(uint8_t value);
};

class AnalogInputPin : public BasePin {
private:
  static const uint64_t MAX_READ_VALUE = 1023;
  size_t scale;

public:
  static const size_t NO_SCALE = 0;
  AnalogInputPin(uint8_t pin, size_t scale = AnalogInputPin::NO_SCALE);
  double read();
  size_t scaledRead();
};
