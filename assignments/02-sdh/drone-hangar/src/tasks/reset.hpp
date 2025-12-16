#pragma once

#include "core/tasks.hpp"
#include "io/button.hpp"

class ResetTask : public Task<ResetTask> {
 private:
  Button* resetButton;
  GlobalState prevState;

  class IdleState : public TaskState<ResetTask> {
   public:
    void step(ResetTask* task, Context* context) override;
  };

  class WaitResetState : public TaskState<ResetTask> {
   public:
    void step(ResetTask* task, Context* context) override;
  };

 public:
  ResetTask(Button* resetButton, GlobalState initialState);
};
