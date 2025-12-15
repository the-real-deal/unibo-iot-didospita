#pragma once

#include "scheduler.hpp"
#include <stdint.h>

template <class T> class TaskState {
public:
  virtual void step(T *task, Context *context) = 0;
  virtual ~TaskState() = default;
};

template <class T> class Task : public LogicThread {
public:
  TaskState<T> *state;
  TaskState<T> *prevState;

  Task(TaskState<T> *initialState) : state(initialState), prevState(nullptr) {};
  ~Task() {
    delete this->state;
    delete this->prevState;
  };

  void step(Context *context) override {
    delete this->prevState;
    this->prevState = nullptr;
    this->state->step(static_cast<T *>(this), context);
  };

  void switchState(TaskState<T> *state) {
    assert(this->prevState == nullptr) // prevent calling twice
        this->prevState =
        this->state; // state deleted on next read to prevent
                     // deletion while the step is still executing
    this->state = state;
  }
};
