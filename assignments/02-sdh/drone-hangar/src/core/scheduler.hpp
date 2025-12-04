#pragma once

#include "timer.hpp"
#include <LinkedList.h>
#include <stdint.h>

class Scheduler;

// interfaces to not work directly with tasks and sensors to avoid generics
class LogicThread {
public:
  virtual void step(uint64_t elapsedTime) = 0;
  virtual ~LogicThread() = default;
};

class ExternalInput {
public:
  virtual void read() = 0;
  virtual ~ExternalInput() = default;
};

class Scheduler {
private:
  Timer timer;
  LinkedList<ExternalInput *> inputs;
  LinkedList<LogicThread *> threads;

public:
  Scheduler(int period);
  void addInput(ExternalInput *input);
  void addThread(LogicThread *thread);
  void advance();
};
