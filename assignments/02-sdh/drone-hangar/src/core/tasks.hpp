#pragma once

#include "scheduler.hpp"
#include <stdint.h>

template <class T> class TaskAction {
public:
  virtual void step(T *task, SchedulerContext *context) = 0;
  virtual ~TaskAction() = default;
};

template <class T> class Task : public LogicThread {
public:
  TaskAction<T> *action;

  Task(TaskAction<T> *initialAction) : action(initialAction) {};
  ~Task() { delete this->action; };

  void step(SchedulerContext *context) override {
    this->action->step(static_cast<T *>(this), context);
  };

  void switchAction(TaskAction<T> *action) {
    delete this->action;
    this->action = action;
  }
};
