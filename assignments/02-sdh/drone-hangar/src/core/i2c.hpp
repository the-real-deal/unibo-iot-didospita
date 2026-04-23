#pragma once

#include <stdint.h>

#include "begin.hpp"

class I2CManager : public DelayedBegin
{
private:
  uint8_t startAddress;
  static const uint8_t MIN_ADDRESS = 1;
  static const uint8_t MAX_ADDRESS = 126;

public:
  void begin() override;
  I2CManager();
  uint8_t scan();
};
