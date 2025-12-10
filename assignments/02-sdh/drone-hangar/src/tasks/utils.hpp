#pragma once

#include "core/tasks.hpp"

template <typename T>
void blockOnAlarm(T *task, StateManager *stateManager,
                  TaskAction<T> *returnAction) {
  if (stateManager->getState() == StateType::ALARM) {
    task->switchAction(
        new BlockedTaskAction<LCDTask, StateType, LCDTask::IdleAction>(
            StateType::ALARM, returnAction));
  }
}