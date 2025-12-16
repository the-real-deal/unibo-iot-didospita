#pragma once

#include "core/tasks.hpp"

template <typename T, typename B, typename S>
class BlockedTaskState : public TaskState<T> {
 private:
  B blockingState;
  S* (*returnStateCreator)();

 public:
  BlockedTaskState(B blockingState, S* (*returnStateCreator)())
      : blockingState(blockingState), returnStateCreator(returnStateCreator) {}
  void step(T* task, Context* context) override {
    if (context->getState() == blockingState) {
      return;
    }
    task->switchState(returnStateCreator());
  }
};

template <typename T, typename S>
void blockOnAlarm(T* task, Context* context, S* (*returnStateCreator)()) {
  if (context->getState() == GlobalState::Alarm) {
    task->switchState(new BlockedTaskState<T, GlobalState, S>(
        GlobalState::Alarm, returnStateCreator));
  }
}
template <typename T, typename S>
void blockOnAlarm(T* task, Context* context) {
  blockOnAlarm(task, context, static_cast<S* (*)()>([]() { return new S(); }));
}
