#include "lcd.hpp"

LCD::LCD(uint8_t address, uint8_t cols, uint8_t rows)
    : lcd(address, cols, rows), currentCol(0), currentRow(0) {
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

void LCD::print(String message) {
  size_t start = 0;
  size_t length = message.length();
  size_t end;
  do {
    int newLineIndex = message.indexOf('\n');
    end = newLineIndex == -1 ? length : newLineIndex;
    String row = message.substring(start, end);
    this->lcd.print(row);
    this->lcd.setCursor(0, this->row() + 1);
    start = end + 1;
  } while (end < length);
}