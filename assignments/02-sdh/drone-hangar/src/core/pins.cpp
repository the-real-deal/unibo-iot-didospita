#include "pins.hpp"

BasePin::BasePin(const uint8_t pin, const PinType type) : pin(pin), type(type) {
  pinMode(this->pin, static_cast<uint8_t>(type));
}

uint8_t BasePin::getPin() { return this->pin; }

PinType BasePin::getType() { return this->type; }

DigitalOutputPin::DigitalOutputPin(const uint8_t pin)
    : BasePin(pin, PinType::Output) {}

void DigitalOutputPin::write(const DigitalValue value) {
  digitalWrite(this->pin, static_cast<uint8_t>(value));
}

DigitalInputPin::DigitalInputPin(const uint8_t pin)
    : BasePin(pin, PinType::Input) {}

DigitalValue DigitalInputPin::read() {
  return static_cast<DigitalValue>(digitalRead(this->pin));
}

AnalogOutputPin::AnalogOutputPin(const uint8_t pin)
    : BasePin(pin, PinType::Output) {}

void AnalogOutputPin::write(const uint8_t value) {
  analogWrite(this->pin, value);
}

AnalogInputPin::AnalogInputPin(const uint8_t pin, const size_t scale)
    : BasePin(pin, PinType::Input), scale(scale) {}

double AnalogInputPin::read() {
  return static_cast<double>(analogRead(this->pin)) /
         static_cast<double>(AnalogInputPin::MAX_READ_VALUE);
}

size_t AnalogInputPin::scaledRead() {
  return floor(this->read() * this->scale);
}
