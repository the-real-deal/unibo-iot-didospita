#pragma once

#include "core/tasks.hpp"
#include "io/indicator.hpp"

class BlinkTask : public Task<BlinkTask> {
 private:
  Indicator* blinkIndicator;
  uint64_t periodMillis;

  class IdleState : public TaskState<BlinkTask> {
   public:
    void step(BlinkTask* task, Context* context) override;
  };

  class OnState : public TaskState<BlinkTask> {
   private:
    Timer timer;

   public:
    OnState(BlinkTask* task);
    void step(BlinkTask* task, Context* context) override;
  };

  class OffState : public TaskState<BlinkTask> {
   private:
    Timer timer;

   public:
    OffState(BlinkTask* task);
    void step(BlinkTask* task, Context* context) override;
  };

 public:
  BlinkTask(Indicator* blinkIndicator, uint64_t periodMillis);
};
