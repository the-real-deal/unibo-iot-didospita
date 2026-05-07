#pragma once

#include <stdint.h>

class Timer
{
private:
  uint32_t periodMillis;
  uint32_t startTime;

public:
  Timer(uint32_t periodMillis);
  Timer();
  uint32_t getElapsedMillis();
  bool isFinished();
  void start();
  uint32_t wait();
};
