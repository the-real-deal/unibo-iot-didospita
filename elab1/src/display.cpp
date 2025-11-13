#include "display.hpp"
#include "config.h"

LiquidCrystal_I2C *createDisplay(const uint8_t address, const uint8_t cols,
                                 const uint8_t rows) {
  auto lcd = new LiquidCrystal_I2C(address, cols, rows);
  lcd->init();
  lcd->backlight();
  return lcd;
}