#include "led.hpp"

Led::Led(uint8_t pin) : pin(pin), value(DigitalValue::Low) {}

void Led::setValue(DigitalValue value) {
  this->pin.write(value);
  this->value = value;
}

void Led::turnOn() { this->setValue(DigitalValue::High); }

void Led::turnOff() { this->setValue(DigitalValue::Low); }

bool Led::isOn() { return this->value == DigitalValue::High; }

bool Led::isOff() { return this->value == DigitalValue::Low; }
