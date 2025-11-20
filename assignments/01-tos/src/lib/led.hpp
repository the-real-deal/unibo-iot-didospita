#pragma once

#include <stdint.h>
#include <stdlib.h>

void turnOnLed(const uint8_t pin);
void turnOffLed(const uint8_t pin);
void fadeLed(const uint8_t pin, int32_t *const currentFade);
