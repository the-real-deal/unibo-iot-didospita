#pragma once

#include <stdint.h>

#include "setup.hpp"

class I2CManager : public Setup
{
private:
  uint8_t startAddress;

public:
  void setup() override;
  I2CManager();
  uint8_t scan();
};
