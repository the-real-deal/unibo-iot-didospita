#include "pir.hpp"

PIR::PIR(uint8_t pin) : pin(pin) {}

void PIR::read() { this->value = this->pin.read(); }
