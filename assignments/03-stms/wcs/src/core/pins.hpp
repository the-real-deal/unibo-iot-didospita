#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "setup.hpp"

enum class PinType : uint8_t
{
  Input = INPUT,
  Output = OUTPUT,
};

class BasePin : public Setup
{
protected:
  uint8_t pin;
  PinType type;
  BasePin(uint8_t pin, PinType type);

public:
  void setup() override;
  uint8_t getPin();
  PinType getType();
};

enum class DigitalValue : uint8_t
{
  High = HIGH,
  Low = LOW,
};

class DigitalOutputPin : public BasePin
{
public:
  DigitalOutputPin(uint8_t pin);
  void write(DigitalValue status);
};

class DigitalInputPin : public BasePin
{
public:
  DigitalInputPin(uint8_t pin);
  DigitalValue read();
  uint32_t readPulse(DigitalValue value);
  uint32_t readPulse(DigitalValue value, uint32_t timeout);
};

class AnalogOutputPin : public BasePin
{
public:
  AnalogOutputPin(uint8_t pin);
  void write(uint8_t value);
};

class AnalogInputPin : public BasePin
{
private:
  static const uint32_t MAX_READ_VALUE = 1023;

public:
  AnalogInputPin(uint8_t pin);
  double read();
};
