#ifndef I2C_H
#define I2C_H

#include <stdint.h>

typedef void (*i2c_func_t)(uint8_t address);

void i2c_scan(i2c_func_t callback);

#endif // I2C_H