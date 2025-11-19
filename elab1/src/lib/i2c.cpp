#include "i2c.hpp"
#include <Arduino.h>
#include <Wire.h>

void i2cScan(const i2cScanCallback_t callback) {
  Serial.println("\nI2C Scanner");
  int found = 0;
  while (found == 0) {
    Serial.println("Scanning...");
    for (uint8_t address = 1; address < 127; address++) {
      Wire.beginTransmission(address);
      const uint8_t error = Wire.endTransmission();
      if (error == 0) {
        Serial.print("I2C device found at address 0x");
        if (address < 16)
          Serial.print("0");
        Serial.print(address, HEX);
        Serial.println("!");
        callback(address);
        found = 1;
      } else if (error == 4) {
        Serial.print("Unknown error at address 0x");
        if (address < 16)
          Serial.print("0");
        Serial.println(address, HEX);
      }
    }
    Serial.flush();
  }
}
