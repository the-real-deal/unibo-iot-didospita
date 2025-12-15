#pragma once

#include "core/tasks.hpp"
#include "io/presence.hpp"

class DPDTask : public Task<DPDTask> {
private:
  PresenceSensor *dronePresenceSensor;

  class IdleState : public TaskState<DPDTask> {
  public:
    void step(DPDTask *task, SchedulerContext *context) override;
  };

  class ReadingState : public TaskState<DPDTask> {
  public:
    void step(DPDTask *task, SchedulerContext *context) override;
  };

public:
  DPDTask(PresenceSensor *dronePresenceSensor);
};
