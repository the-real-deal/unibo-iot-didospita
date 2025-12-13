#pragma once

#include "core/tasks.hpp"
#include <LiquidCrystal_I2C.h>

class LCDTask : public Task<LCDTask> {
private:
  LiquidCrystal_I2C *lcd;

  class PrintState : public TaskState<LCDTask> {
  public:
    void step(LCDTask *task, SchedulerContext *context) override;
  };

  class IdleState : public TaskState<LCDTask> {
  public:
    void step(LCDTask *task, SchedulerContext *context) override;
  };

public:
  LCDTask(LiquidCrystal_I2C *lcd);
};
