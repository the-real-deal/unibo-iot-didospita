#pragma once

#include <Arduino.h>
#include <stdint.h>
#include <EnableInterrupt.h>

#include "setup.hpp"

enum class PinType : uint8_t
{
  Input = INPUT,
  Output = OUTPUT,
};

class IOPin : public Setup
{
protected:
  uint8_t pin;
  PinType type;
  IOPin(uint8_t pin, PinType type);

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

class DigitalOutputPin : public IOPin
{
public:
  DigitalOutputPin(uint8_t pin);
  void write(DigitalValue status);
};

class DigitalInputPin : public IOPin
{
public:
  DigitalInputPin(uint8_t pin);
  DigitalValue read();
  uint32_t readPulse(DigitalValue value);
  uint32_t readPulse(DigitalValue value, uint32_t timeout);
};

class AnalogOutputPin : public IOPin
{
public:
  AnalogOutputPin(uint8_t pin);
  void write(uint8_t value);
};

class AnalogInputPin : public IOPin
{
private:
  static const uint32_t MAX_READ_VALUE = 1023;

public:
  AnalogInputPin(uint8_t pin);
  double read();
};

enum class InterruptMode : uint8_t
{
  Rising = RISING,
  Falling = FALLING,
  Change = CHANGE,
  High = HIGH,
  Low = LOW,
};

class InterruptPin : public Setup
{
private:
  uint8_t pin;
  InterruptMode mode;
  interruptFunctionType handler;

public:
  InterruptPin(uint8_t pin, InterruptMode mode, interruptFunctionType handler);
  virtual void setup() override;
  uint8_t getPin();
  InterruptMode getMode();
  interruptFunctionType getHandler();
};
