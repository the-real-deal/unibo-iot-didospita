#include "button.hpp"

PushButton::PushButton(uint8_t pin) : pin(pin), pressed(false) {}

void PushButton::read() {
  this->pressed = this->pin.read() == DigitalValue::High;
}

bool PushButton::isPressed() { return this->pressed; }
