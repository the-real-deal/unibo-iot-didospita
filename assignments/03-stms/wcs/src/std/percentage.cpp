#include "percentage.hpp"

#include <Arduino.h>

double toPercentage(int value, int max) {
    return min(((double)value / (double)max), 1.0);
}

int fromPercentage(double value, int max) {
    return max * value;
}
