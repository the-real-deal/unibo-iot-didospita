#pragma once

#include "core/tasks.hpp"
#include "io/button.hpp"

class ResetTask : public Task<ResetTask>
{
private:
  Button *resetButton;
  GlobalState prevState;

  class IdleState : public TaskState<ResetTask>
  {
  public:
    void step(ResetTask *task, Context *context) override;
  };
  IdleState idleState;

  class WaitResetState : public TaskState<ResetTask>
  {
  public:
    void step(ResetTask *task, Context *context) override;
  };
  WaitResetState waitResetState;

public:
  ResetTask(Button *resetButton, GlobalState initialState);
};
