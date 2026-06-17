#include "percentage.hpp"

#include <Arduino.h>

double toPercentage(int value, int maxValue) {
    return (double)min(max(0, value), maxValue) / (double)maxValue;
}

int fromPercentage(double value, int maxValue) {
    return maxValue * max(0, min(value, 1.0));
}
