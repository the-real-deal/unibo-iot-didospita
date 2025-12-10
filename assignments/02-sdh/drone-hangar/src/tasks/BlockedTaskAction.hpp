#pragma once

#include "core/state.hpp"
#include "core/tasks.hpp"

template <typename T, typename B, typename A>
class BlockedTaskAction : public TaskAction<T> {
private:
  B blockingState;
  A *returningAction;

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