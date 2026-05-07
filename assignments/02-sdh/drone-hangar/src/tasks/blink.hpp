#pragma once

#include "core/tasks.hpp"
#include "io/indicator.hpp"

class BlinkTask : public Task<BlinkTask>
{
private:
  Indicator *blinkIndicator;
  uint32_t periodMillis;
  Timer timer;

  class IdleState : public TaskState<BlinkTask>
  {
  public:
    void step(BlinkTask *task, Context *context) override;
  };
  static IdleState IDLE;

  class OnState : public TaskState<BlinkTask>
  {
  public:
    void setup(BlinkTask *task) override;
    void step(BlinkTask *task, Context *context) override;
  };
  static OnState ON;

  class OffState : public TaskState<BlinkTask>
  {
  public:
    void setup(BlinkTask *task) override;
    void step(BlinkTask *task, Context *context) override;
  };
  static OffState OFF;

public:
  BlinkTask(Indicator *blinkIndicator, uint32_t periodMillis);
};
