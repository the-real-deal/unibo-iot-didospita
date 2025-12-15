#pragma once

#include "scheduler.hpp"
#include <stdint.h>

template <class T> class TaskState {
public:
  virtual void step(T *task, SchedulerContext *context) = 0;
  virtual ~TaskState() = default;
};

template <class T> class Task : public LogicThread {
public:
  TaskState<T> *state;

  Task(TaskState<T> *initialState) : state(initialState) {};
  ~Task() { delete this->state; };

  void step(SchedulerContext *context) override {
    this->state->step(static_cast<T *>(this), context);
  };

  void switchState(TaskState<T> *state) {
    delete this->state;
    this->state = state;
  }
};
