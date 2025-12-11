#pragma once

#include "core/tasks.hpp"

template <typename T, typename B, typename A>
class BlockedTaskAction : public TaskAction<T> {
private:
  B blockingState;
  A *returningAction; // just holds the reference to pass to switchAction

public:
  BlockedTaskAction(B blockingState, A *returningAction)
      : blockingState(blockingState), returningAction(returningAction) {}
  void step(T *task, SchedulerContext *context) override {
    if (context->state() == blockingState) {
      return;
    }
    task->switchAction(returningAction);
  }
};

template <typename T>
void blockOnAlarm(T *task, SchedulerContext *context,
                  TaskAction<T> *returnAction) {
  if (context->state() == StateType::Alarm) {
    Serial.println("ALARM");
    task->switchAction(new BlockedTaskAction<T, StateType, TaskAction<T>>(
        StateType::Alarm, returnAction));
  }
}
