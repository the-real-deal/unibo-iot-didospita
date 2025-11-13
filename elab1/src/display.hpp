#ifndef DISPLAY_H
#define DISPLAY_H

#include <LiquidCrystal_I2C.h>
#include <stdint.h>

LiquidCrystal_I2C *createDisplay(uint8_t address, uint8_t cols, uint8_t rows);

#endif // DISPLAY_H