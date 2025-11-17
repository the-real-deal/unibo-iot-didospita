#include "potentiometer.hpp"
#include <Arduino.h>

#define MAX_ANALOG_VAL 1024

size_t readPotentiometer(const uint8_t pin, const uint8_t scale) {
  const int32_t value = analogRead(pin);
  return floor(((double)value) * scale / MAX_ANALOG_VAL);
}