#include "pins.hpp"
#include <assert.h>

BasePin::BasePin(uint8_t pin, PinType type) : pin(pin), type(type) {
  pinMode(this->pin, static_cast<uint8_t>(type));
}

uint8_t BasePin::getPin() { return this->pin; }

PinType BasePin::getType() { return this->type; }

DigitalOutputPin::DigitalOutputPin(uint8_t pin)
    : BasePin(pin, PinType::Output) {}

void DigitalOutputPin::write(DigitalValue value) {
  digitalWrite(this->pin, static_cast<uint8_t>(value));
}

DigitalInputPin::DigitalInputPin(uint8_t pin) : BasePin(pin, PinType::Input) {}

DigitalValue DigitalInputPin::read() {
  return static_cast<DigitalValue>(digitalRead(this->pin));
}

uint64_t DigitalInputPin::readPulse(DigitalValue value) {
  return pulseIn(this->pin, static_cast<uint8_t>(value));
}

uint64_t DigitalInputPin::readPulse(DigitalValue value, uint64_t timeout) {
  return pulseIn(this->pin, static_cast<uint8_t>(value), timeout);
}

AnalogOutputPin::AnalogOutputPin(uint8_t pin) : BasePin(pin, PinType::Output) {}

void AnalogOutputPin::write(uint8_t value) { analogWrite(this->pin, value); }

AnalogInputPin::AnalogInputPin(uint8_t pin, size_t scale)
    : BasePin(pin, PinType::Input), scale(scale) {}

double AnalogInputPin::read() {
  return static_cast<double>(analogRead(this->pin)) /
         static_cast<double>(AnalogInputPin::MAX_READ_VALUE);
}

size_t AnalogInputPin::scaledRead() {
  assert(this->scale != AnalogInputPin::NO_SCALE);
  return floor(this->read() * this->scale);
}
