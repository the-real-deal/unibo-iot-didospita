#pragma once

#include "game.hpp"
#include <stdint.h>

void buttonPressed(const uint8_t pin, Game *const game, const size_t index);
