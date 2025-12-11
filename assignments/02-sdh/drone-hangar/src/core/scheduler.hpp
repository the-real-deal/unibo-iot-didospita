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
  Timer _timer;
  T _state;
  T _previousState;
  T _stateCandidate;
  bool _hasPreviousState;
  uint64_t _elapsed;
  Context(int period, T initialState)
      : _timer(period), _state(initialState), _previousState(initialState),
        _hasPreviousState(false), _elapsed(0) {}

  void waitTimer() { this->_elapsed = this->_timer.wait(); }

  void switchState() {
    this->_previousState = this->_state;
    this->_state = this->_stateCandidate;
    this->_hasPreviousState = true;
  };

public:
  T state() { return this->_state; };
  T previousState() {
    assert(this->hasPreviousState());
    return this->_previousState;
  };
  bool hasPreviousState() { return this->_hasPreviousState; };
  void setState(T state) { this->_stateCandidate = state; };
  uint64_t elapsed() { return this->_elapsed; };
};

using SchedulerContext = Context<StateType>;

// interfaces to not work directly with tasks and sensors to avoid generics
class LogicThread {
public:
  virtual void step(SchedulerContext *context) = 0;
  virtual ~LogicThread() = default;
};

class ExternalInput {
public:
  virtual void read() = 0;
  virtual ~ExternalInput() = default;
};

class Scheduler {
private:
  SchedulerContext context;
  LinkedList<ExternalInput *> inputs;
  LinkedList<LogicThread *> threads;

public:
  Scheduler(int period, StateType initialState);
  void addInput(ExternalInput *input);
  void addThread(LogicThread *thread);
  void advance();
};
