#pragma once

#include <stdint.h>

#include "scheduler.hpp"
#include "utils.hpp"

template <class T>
class TaskState {
 public:
  virtual void step(T* task, Context* context) = 0;
  virtual ~TaskState() = default;
};

template <class T>
class Task : public LogicThread {
 public:
  TaskState<T>* state;
  TaskState<T>* prevState;

  Task(TaskState<T>* initialState) : state(initialState), prevState(nullptr) {};
  ~Task() {
    safeDelete(&this->state);
    safeDelete(&this->prevState);
  };

  void step(Context* context) override {
    safeDelete(&this->prevState);
    this->state->step(static_cast<T*>(this), context);
  };

  void switchState(TaskState<T>* state) {
    assert(this->prevState == nullptr);  // prevent calling twice
    this->prevState =
        this->state;  // state deleted on next read to prevent
                      // deletion while the step is still executing
    this->state = state;
  }
};
