#pragma once

#include <LiquidCrystal_I2C.h>

#include "kernel/setup.hpp"

#ifndef LCD_COLS
#define LCD_COLS 16
#endif

#ifndef LCD_ROWS
#define LCD_ROWS 2
#endif

class LCD
{
private:
  LiquidCrystal_I2C lcd;
  
  public:
  LCD();
  void begin(uint8_t address);
  void setCursor(uint8_t col, uint8_t row);
  void clear();
  void clearRow(uint8_t row);
  void print(const char *message);
};