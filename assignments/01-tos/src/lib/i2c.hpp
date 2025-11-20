#pragma once

#include <stdint.h>

typedef void (*i2cScanCallback_t)(const uint8_t address);

void i2cScan(const i2cScanCallback_t callback);
