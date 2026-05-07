#pragma once

#include <Arduino.h>
#include <assert.h>
#include <stdint.h>

#include "config.h"
#include "state.hpp"
#include "setup.hpp"
#include "timer.hpp"

class Scheduler;

class Context
{
  friend Scheduler;

private:
  GlobalState state;
  GlobalState stateCandidate;
  uint32_t elapsedMillis;

public:
  Context(GlobalState initialState);
  GlobalState getState();
  void setState(GlobalState state);
  uint32_t getElapsedMillis();
};

class LogicThread : public Setup
{
  friend Scheduler;

protected:
  virtual void step(Context *context) = 0;

public:
  virtual ~LogicThread() = default;
};

class ExternalInput : public Setup
{
  friend Scheduler;

protected:
  virtual void read() = 0;

public:
  virtual ~ExternalInput() = default;
};

class Scheduler : public Setup
{
private:
  Context context;
  Timer timer;
  ExternalInput *inputs[SCHEDULER_MAX_INPUTS];
  size_t n_inputs;
  LogicThread *threads[SCHEDULER_MAX_THREADS];
  size_t n_threads;

public:
  Scheduler(int periodMillis, GlobalState initialState);
  void addInput(ExternalInput *input);
  void addThread(LogicThread *thread);
  void setup() override;
  void advance();
};
