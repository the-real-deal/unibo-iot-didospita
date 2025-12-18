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
  TaskState<T>* stateCandidate;

  Task(TaskState<T>* initialState)
      : state(initialState), stateCandidate(nullptr) {};
  ~Task() {
    delete this->state;
    delete this->stateCandidate;
  };

  void step(Context* context) override {
    // TODO: check if state candidate works
    this->stateCandidate = nullptr;
    this->state->step(static_cast<T*>(this), context);
    if (this->stateCandidate != nullptr) {
      delete this->state;
      this->state = this->stateCandidate;
    }
  };

  void switchState(TaskState<T>* state) { this->stateCandidate = this->state; }
};
