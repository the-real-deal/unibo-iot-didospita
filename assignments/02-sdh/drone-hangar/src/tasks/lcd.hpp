#pragma once

#include "core/tasks.hpp"
#include <LiquidCrystal_I2C.h>

class LCDTask : public Task<LCDTask> {
private:
  LiquidCrystal_I2C *lcd; // just holds the reference, does not own it

  class PrintStateAction : public TaskAction<LCDTask> {
  public:
    void step(LCDTask *task, SchedulerContext *context) override;
  };

  class IdleAction : public TaskAction<LCDTask> {
  public:
    void step(LCDTask *task, SchedulerContext *context) override;
  };

public:
  LCDTask(LiquidCrystal_I2C *lcd);
};
