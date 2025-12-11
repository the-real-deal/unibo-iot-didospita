#pragma once

#include "core/tasks.hpp"

template <typename T, typename B, typename A>
class BlockedTaskState : public TaskState<T> {
private:
  B blockingState;
  A *returnTo;

public:
  BlockedTaskState(B blockingState, A *returnTo)
      : blockingState(blockingState), returnTo(returnTo) {}
  void step(T *task, SchedulerContext *context) override {
    if (context->getState() == blockingState) {
      return;
    }
    task->switchTo(returnTo);
  }
};

template <typename T>
void blockOnAlarm(T *task, SchedulerContext *context, TaskState<T> *returnTo) {
  if (context->getState() == GlobalState::Alarm) {
    Serial.println("ALARM");
    task->switchTo(new BlockedTaskState<T, GlobalState, TaskState<T>>(
        GlobalState::Alarm, returnTo));
  }
}
