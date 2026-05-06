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
  static IdleState IDLE;

  class WaitResetState : public TaskState<ResetTask>
  {
  public:
    void step(ResetTask *task, Context *context) override;
  };
  static WaitResetState WAIT_RESET;

public:
  ResetTask(Button *resetButton, GlobalState initialState);
};
