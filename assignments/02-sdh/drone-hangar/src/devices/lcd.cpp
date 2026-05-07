#include "lcd.hpp"

#include <Arduino.h>

LCD::LCD(): lcd(0, 0, 0) {}

void LCD::begin(uint8_t address, uint8_t cols, uint8_t rows)
{
  this->lcd = LiquidCrystal_I2C(address, cols, rows);
  this->lcd.init();
  this->lcd.backlight();
  this->lcd.clear();
}

void LCD::setCursor(uint8_t col, uint8_t row)
{
  this->lcd.setCursor(col, row);
}

void LCD::clear()
{
  this->lcd.clear();
  this->setCursor(0, 0);
}

void LCD::print(const char *message)
{
  this->lcd.print(message);
}