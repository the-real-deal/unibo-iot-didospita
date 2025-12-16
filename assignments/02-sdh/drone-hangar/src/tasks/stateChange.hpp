#pragma once

#include "core/tasks.hpp"
#include "io/display.hpp"
#include "io/message.hpp"

class StateChangeTask : public Task<StateChangeTask> {
 private:
  Display* internalDisplay;
  MessageService* messageService;
  GlobalState prevState;

  class IdleState : public TaskState<StateChangeTask> {
   public:
    void step(StateChangeTask* task, Context* context) override;
  };

  class PrintState : public TaskState<StateChangeTask> {
   public:
    void step(StateChangeTask* task, Context* context) override;
  };

 public:
  StateChangeTask(Display* internalDisplay, MessageService* messageService,
                  GlobalState initialState);
};
