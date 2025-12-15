#pragma once

#include "core/tasks.hpp"
#include <LiquidCrystal_I2C.h>

class LCDTask : public Task<LCDTask> {
private:
  LiquidCrystal_I2C *lcd;

  class IdleState : public TaskState<LCDTask> {
  public:
    void step(LCDTask *task, Context *context) override;
  };

  class PrintState : public TaskState<LCDTask> {
  public:
    void step(LCDTask *task, Context *context) override;
  };

public:
  LCDTask(LiquidCrystal_I2C *lcd);
};
