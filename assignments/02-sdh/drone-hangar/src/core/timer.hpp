#pragma once

#include <stdint.h>

class Timer {
private:
  uint64_t period;
  uint64_t startTime;
  bool running;

public:
  Timer(uint64_t period);
  void start();
  void stop();
  uint64_t elapsed();
  uint64_t wait();
};
