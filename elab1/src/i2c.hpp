#ifndef I2C_HPP
#define I2C_HPP

#include <stdint.h>

typedef void (*i2cScanCallback_t)(uint8_t address);

void i2cScan(i2cScanCallback_t callback);

#endif // I2C_HPP
