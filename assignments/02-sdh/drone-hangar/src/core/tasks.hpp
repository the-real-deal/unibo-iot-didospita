#pragma once

#include "scheduler.hpp"
#include <stdint.h>

template <class T> class Task;
template <class T> class TaskAction;

template <class T> class TaskAction {
public:
  virtual void step(T *task, uint64_t elapsedTime) = 0;
  virtual ~TaskAction() = default;
};

template <class T> class Task : public LogicThread {
public:
  TaskAction<T> *action;

  Task(TaskAction<T> *initialAction) { this->switchAction(initialAction); };

  void step(uint64_t elapsedTime) override {
    this->action->step(static_cast<T *>(this), elapsedTime);
  };

  void switchAction(TaskAction<T> *action) { this->action = action; }
};
