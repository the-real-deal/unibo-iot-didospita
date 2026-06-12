#pragma once

#include <stdint.h>

#include "kernel/setup.hpp"

#ifndef I2C_MIN_ADDRESS
#define I2C_MIN_ADDRESS 0
#endif

#ifndef I2C_MAX_ADDRESS
#define I2C_MAX_ADDRESS 126
#endif

class I2CManager : public Setup
{
private:
  uint8_t startAddress;

public:
  void setup() override;
  I2CManager();
  uint8_t scan();
};
