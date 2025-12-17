#pragma once

#include <LinkedList.h>
// #include <assert.h>
#include <stdint.h>

#include "state.hpp"
#include "timer.hpp"

class Scheduler;

class Context {
  friend Scheduler;

 private:
  Timer timer;
  GlobalState state;
  GlobalState stateCandidate;
  uint64_t elapsedMillis;

  void waitTimer();
  void switchState();

 public:
  Context(int periodMillis, GlobalState initialState);
  GlobalState getState();
  void setState(GlobalState state);
  uint64_t getElapsedMillis();
};

// interfaces to not work directly with tasks and sensors to avoid generics
class LogicThread {
 public:
  virtual void step(Context* context) = 0;
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
  Context context;
  LinkedList<ExternalInput*> inputs;
  LinkedList<LogicThread*> threads;

 public:
  Scheduler(int periodMillis, GlobalState initialState);
  void addInput(ExternalInput* input);
  void addThread(LogicThread* thread);
  void advance();
};
