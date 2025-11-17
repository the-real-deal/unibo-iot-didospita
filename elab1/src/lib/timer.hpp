#pragma once

#include <stdint.h>

struct Timer {
  uint64_t period;
  uint64_t startTime;
};

Timer initTimer(const uint64_t period);
bool timerEnded(const Timer *const timer);
