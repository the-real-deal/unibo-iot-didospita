#include "lcd.hpp"

#include <Arduino.h>

LCD::LCD(uint8_t address, uint8_t cols, uint8_t rows)
    : lcd(address, cols, rows), currentCol(0), currentRow(0) {}

void LCD::begin() {
  this->lcd.init();
  this->lcd.backlight();
  this->clear();
}

void LCD::setCursor(uint8_t col, uint8_t row) {
  this->lcd.setCursor(col, row);
  this->currentCol = col;
  this->currentRow = row;
}

uint8_t LCD::col() { return this->currentCol; }

uint8_t LCD::row() { return this->currentRow; }

void LCD::clear() {
  this->lcd.clear();
  this->setCursor(0, 0);
}

void LCD::print(String* message) {
  this->lcd.print(*message);
}