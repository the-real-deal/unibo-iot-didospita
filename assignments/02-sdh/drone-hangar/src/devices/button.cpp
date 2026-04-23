#include "button.hpp"

PushButton::PushButton(DigitalInputPin pin) : pin(pin), pressed(false) {}

void PushButton::read()
{
  this->pressed = this->pin.read() == DigitalValue::High;
}

bool PushButton::isPressed() { return this->pressed; }
