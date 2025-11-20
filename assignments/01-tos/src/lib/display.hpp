#pragma once

#include <LiquidCrystal_I2C.h>
#include <stdint.h>

LiquidCrystal_I2C *createDisplay(const uint8_t address, const uint8_t cols,
                                 const uint8_t rows);
