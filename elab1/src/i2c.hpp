#pragma once

#include <stdint.h>

typedef void (*i2cScanCallback_t)(uint8_t address);

void i2cScan(i2cScanCallback_t callback);
