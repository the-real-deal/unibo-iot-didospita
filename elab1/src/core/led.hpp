#pragma once

#include <stdint.h>

void turnOnAllGameLeds();
void turnOffAllGameLeds();
void fadeLed(const uint8_t pin, int32_t *const currentFade);
