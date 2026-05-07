#include "i2c.hpp"

#include <Arduino.h>
#include <Wire.h>

#include "config.h"

I2CManager::I2CManager() : startAddress(I2C_MIN_ADDRESS) {}

void I2CManager::setup()
{
  Wire.begin();
}

uint8_t I2CManager::scan()
{
  for (uint8_t address = this->startAddress; address <= I2C_MAX_ADDRESS;
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
