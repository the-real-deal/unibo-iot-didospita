#include "i2c.hpp"

#include <Arduino.h>
#include <Wire.h>

I2CManager::I2CManager() : startAddress(MIN_ADDRESS) {}

int I2CManager::scan() {
  for (uint8_t address = this->startAddress; address <= MAX_ADDRESS;
       address++) {
    Wire.beginTransmission(address);
    const uint8_t error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
      this->startAddress = address + 1;
      return address;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  return -1;  // no device found
}
