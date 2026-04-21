#include "i2c.hpp"

#include <Arduino.h>
#include <Wire.h>

I2CManager::I2CManager() : startAddress(MIN_ADDRESS) {}

uint8_t I2CManager::scan()
{
  for (uint8_t address = this->startAddress; address <= MAX_ADDRESS;
       address++)
  {
    Wire.beginTransmission(address);
    const uint8_t error = Wire.endTransmission();
    if (error == 0)
    {
      this->startAddress = address + 1;
      return address;
    }
    else if (error == 4)
    {
      exit(error);
    }
  }
  exit(-1); // no device found
}
