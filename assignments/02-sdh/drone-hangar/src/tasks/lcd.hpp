#pragma once

#include "core/tasks.hpp"
#include <LiquidCrystal_I2C.h>

class LCDTask : public Task<LCDTask> {
private:
  LiquidCrystal_I2C *lcd; // just holds the reference, does not own it
  class PrintStateAction : public TaskAction<LCDTask> {
  public:
    void step(LCDTask *task, uint64_t elapsedTime,
              StateManager *stateManager) override;
  };
  class IdleAction : public TaskAction<LCDTask> {
  public:
    void step(LCDTask *task, uint64_t elapsedTime,
              StateManager *stateManager) override;
  };

public:
  LCDTask(LiquidCrystal_I2C *lcd);
};
