#pragma once

#include <stdint.h>

#include "setup.hpp"

class I2CManager : public Setup
{
private:
  uint8_t startAddress;
  static inline const uint8_t MIN_ADDRESS = 1;
  static inline const uint8_t MAX_ADDRESS = 126;

public:
  void setup() override;
  I2CManager();
  uint8_t scan();
};
