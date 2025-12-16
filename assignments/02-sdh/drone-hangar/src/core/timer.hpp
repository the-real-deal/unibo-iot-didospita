#pragma once

#include <stdint.h>

class Timer {
 private:
  uint64_t periodMillis;
  uint64_t startTime;

 public:
  Timer(uint64_t periodMillis);
  uint64_t getElapsedMillis();
  bool isFinished();
  uint64_t wait();
};
