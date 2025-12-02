#pragma once

#include <Arduino.h>
#include <stdint.h>

enum class PinType : uint8_t {
  Input = INPUT,
  Output = OUTPUT,
};

class BasePin {
protected:
  const uint8_t pin;
  const PinType type;
  BasePin(const uint8_t pin, const PinType type);

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
  DigitalOutputPin(const uint8_t pin);
  void write(const DigitalValue status);
};

class DigitalInputPin : public BasePin {
public:
  DigitalInputPin(const uint8_t pin);
  DigitalValue read();
};

class AnalogOutputPin : public BasePin {
public:
  AnalogOutputPin(const uint8_t pin);
  void write(const uint8_t value);
};

class AnalogInputPin : public BasePin {
private:
  static const uint64_t MAX_READ_VALUE = 1023;
  const size_t scale;

public:
  AnalogInputPin(const uint8_t pin, const size_t scale);
  double read();
  size_t scaledRead();
};
