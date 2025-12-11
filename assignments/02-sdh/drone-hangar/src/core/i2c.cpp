#include "i2c.hpp"

#include <Arduino.h>
#include <Wire.h>

#define I2C_NOT_FOUND -1

int i2cScan() {
  static uint8_t startAddress = 0; // continue search from when you left off
  for (uint8_t address = startAddress; address <= 127; address++) {
    Wire.beginTransmission(address);
    const uint8_t error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
      startAddress = address + 1;
      return address;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  return I2C_NOT_FOUND; // no device found
}
