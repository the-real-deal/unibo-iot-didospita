#pragma once

#include <LiquidCrystal_I2C.h>

#include "io/display.hpp"
#include "core/setup.hpp"

class LCD : public Display
{
private:
  LiquidCrystal_I2C lcd;
  void setCursor(uint8_t col, uint8_t row);

public:
  LCD();
  void begin(uint8_t address, uint8_t cols, uint8_t rows);
  void clear() override;
  void print(const char *message) override;
};