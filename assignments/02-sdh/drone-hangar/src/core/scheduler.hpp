#pragma once

#include "state.hpp"
#include "timer.hpp"
#include <LinkedList.h>
#include <assert.h>
#include <stdint.h>

class Scheduler;

template <typename T> class Context {
  friend Scheduler;

private:
  Timer timer;
  T state;
  T previousState;
  T stateCandidate;
  bool _hasPreviousState;
  uint64_t elapsedTime;

  void waitTimer() { this->elapsedTime = this->timer.wait(); }

  void switchState() {
    this->previousState = this->state;
    this->state = this->stateCandidate;
    this->_hasPreviousState = true;
  };

public:
  Context(int period, T initialState)
      : timer(period), state(initialState), previousState(initialState),
        _hasPreviousState(false), elapsedTime(0) {}

  T getState() { return this->state; };
  T getPreviousState() {
    assert(this->hasPreviousState());
    return this->previousState;
  };
  bool hasPreviousState() { return this->_hasPreviousState; };
  void setState(T state) { this->stateCandidate = state; };
  uint64_t getElapsedTime() { return this->elapsedTime; };
};

using SchedulerContext = Context<GlobalState>;

// interfaces to not work directly with tasks and sensors to avoid generics
class LogicThread {
public:
  virtual void step(SchedulerContext *context) = 0;
  virtual ~LogicThread() = default;
};

class ExternalInput {
  friend Scheduler;

protected:
  virtual void read() = 0;

public:
  virtual ~ExternalInput() = default;
};

class Scheduler {
private:
  SchedulerContext context;
  LinkedList<ExternalInput *> inputs;
  LinkedList<LogicThread *> threads;

public:
  Scheduler(int period, GlobalState initialState);
  void addInput(ExternalInput *input);
  void addThread(LogicThread *thread);
  void advance();
};
