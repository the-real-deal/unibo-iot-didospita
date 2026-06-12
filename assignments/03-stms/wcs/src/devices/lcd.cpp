#include "lcd.hpp"

#include <Arduino.h>

LCD::LCD() : lcd(0, 0, 0) {}

void LCD::begin(uint8_t address)
{
  this->lcd = LiquidCrystal_I2C(address, LCD_COLS, LCD_ROWS);
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
}

void LCD::clearRow(uint8_t row) {
  this->lcd.setCursor(0, row);
  for (uint8_t i = 0; i < LCD_COLS; i++)
  {
    this->lcd.print(' ');
  }
  
}

void LCD::print(const char *message)
{
  this->lcd.print(message);
}