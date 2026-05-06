#pragma once

#include "core/tasks.hpp"

template <typename T>
class BlockedTaskState : public TaskState<T>
{

private:
  TaskState<T> *returnState;
  GlobalState blockingState;

public:
  BlockedTaskState(TaskState<T> *returnState, GlobalState blockingState)
      : returnState(returnState), blockingState(blockingState) {}
  BlockedTaskState(TaskState<T> *returnState) : BlockedTaskState(returnState, GlobalState::Alarm) {}

  void step(T *task, Context *context) override
  {
    if (context->getState() == blockingState)
    {
      return;
    }
    task->switchState(returnState);
  }
};

template <typename T>
void blockOnAlarm(T *task, Context *context, BlockedTaskState<T> *blockedTaskState)
{
  if (context->getState() == GlobalState::Alarm)
  {
    task->switchState(blockedTaskState);
  }
}
