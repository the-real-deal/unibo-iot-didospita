#pragma once

#include <LiquidCrystal_I2C.h>

#include "io/display.hpp"

class LCD : public Display
{
private:
  LiquidCrystal_I2C lcd;
  uint8_t currentCol;
  uint8_t currentRow;
  void setCursor(uint8_t col, uint8_t row);
  uint8_t col();
  uint8_t row();

public:
  LCD(LiquidCrystal_I2C lcd);
  void begin();
  void clear() override;
  void print(String *message) override;
};