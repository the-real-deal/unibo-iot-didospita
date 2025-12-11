#include "pir.hpp"

PIRSensor::PIRSensor(uint8_t pin) : pin(pin), value(DigitalValue::Low) {}

void PIRSensor::read() { this->value = this->pin.read(); }

bool PIRSensor::isPresent() { return this->value == DigitalValue::High; }
