#include "led.hpp"
#include "../config.hpp"
#include <Arduino.h>

void turnOnLed(const uint8_t pin) { digitalWrite(pin, HIGH); };

void turnOffLed(const uint8_t pin) { digitalWrite(pin, LOW); };
