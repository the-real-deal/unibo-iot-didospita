#pragma once

#include "blocking.hpp"
#include "core/tasks.hpp"
#include "io/display.hpp"
#include "io/message.hpp"

class StateChangeTask : public Task<StateChangeTask>
{
private:
  Display *internalDisplay;
  MessageService *messageService;
  GlobalState prevState;

  class IdleState : public TaskState<StateChangeTask>
  {
  public:
    void step(StateChangeTask *task, Context *context) override;
  };
  IdleState idleState;
  BlockedTaskState<StateChangeTask> blockedIdleState;

  class PrintState : public TaskState<StateChangeTask>
  {
  public:
    void step(StateChangeTask *task, Context *context) override;
  };
  PrintState printState;

public:
  StateChangeTask(Display *internalDisplay, MessageService *messageService);
};
