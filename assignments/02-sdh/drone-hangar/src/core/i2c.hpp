#pragma once

#include <stdint.h>

class I2CManager {
 private:
  uint8_t startAddress;
  static const uint8_t MIN_ADDRESS = 1;
  static const uint8_t MAX_ADDRESS = 126;

 public:
  I2CManager();
  int scan();
};
