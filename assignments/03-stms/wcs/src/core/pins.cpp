#include "pins.hpp"

#include <assert.h>

IOPin::IOPin(uint8_t pin, PinType type) : pin(pin), type(type) {}

void IOPin::setup()
{
  pinMode(this->pin, static_cast<uint8_t>(type));
}

DigitalOutputPin::DigitalOutputPin(uint8_t pin)
    : IOPin(pin, PinType::Output) {}

void DigitalOutputPin::write(DigitalValue value)
{
  digitalWrite(this->pin, static_cast<uint8_t>(value));
}

DigitalInputPin::DigitalInputPin(uint8_t pin) : IOPin(pin, PinType::Input) {}

DigitalValue DigitalInputPin::read()
{
  return static_cast<DigitalValue>(digitalRead(this->pin));
}

uint32_t DigitalInputPin::readPulse(DigitalValue value)
{
  return pulseIn(this->pin, static_cast<uint8_t>(value));
}

uint32_t DigitalInputPin::readPulse(DigitalValue value, uint32_t timeout)
{
  return pulseIn(this->pin, static_cast<uint8_t>(value), timeout);
}

AnalogOutputPin::AnalogOutputPin(uint8_t pin) : IOPin(pin, PinType::Output) {}

void AnalogOutputPin::write(uint8_t value) { analogWrite(this->pin, value); }

AnalogInputPin::AnalogInputPin(uint8_t pin) : IOPin(pin, PinType::Input) {}

double AnalogInputPin::read()
{
  return static_cast<double>(analogRead(this->pin)) /
         static_cast<double>(AnalogInputPin::MAX_READ_VALUE);
}

InterruptPin::InterruptPin(uint8_t pin, InterruptMode mode, interruptFunctionType handler)
    : pin(pin), mode(mode), handler(handler) {}

void InterruptPin::setup()
{
  enableInterrupt(this->pin, this->handler, static_cast<uint8_t>(this->mode));
}

uint8_t InterruptPin::getPin()
{
  return this->pin;
}

InterruptMode InterruptPin::getMode()
{
  return this->mode;
}

interruptFunctionType InterruptPin::getHandler()
{
  return this->handler;
}
