#pragma once

#include "core/state.hpp"
#include "core/tasks.hpp"

template <typename T, typename B, typename A>
class BlockedTaskAction : public TaskAction<T> {
private:
  B blockingState;
  A *returningAction; // just holds the reference to pass to switchAction

public:
  BlockedTaskAction(B blockingState, A *returningAction)
      : blockingState(blockingState), returningAction(returningAction) {}
  void step(T *task, uint64_t elapsedTime,
            StateManager *stateManager) override {
    if (stateManager->getState() == blockingState) {
      return;
    }
    task->switchAction(returningAction);
  }
};

template <typename T>
void blockOnAlarm(T *task, StateManager *stateManager,
                  TaskAction<T> *returnAction) {
  if (stateManager->getState() == StateType::ALARM) {
    task->switchAction(new BlockedTaskAction<T, StateType, TaskAction<T>>(
        StateType::ALARM, returnAction));
  }
}
