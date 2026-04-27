#pragma once

#include <LiquidCrystal_I2C.h>

#include "io/display.hpp"
#include "core/setup.hpp"

class LCD : public Display, public Setup
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
  void setup() override;
  void clear() override;
  void print(const char *message) override;
};