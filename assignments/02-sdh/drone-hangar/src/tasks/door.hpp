#pragma once

#include "core/tasks.hpp"
#include "devices/servo.hpp"

class DoorTask : public Task<DoorTask> {
private:
  ServoMotor *servo;
  int closedAngle;
  int openAngle;

  class OpenAction : public TaskAction<DoorTask> {
  public:
    void step(DoorTask *task, SchedulerContext *context) override;
  };

  class CloseAction : public TaskAction<DoorTask> {
  public:
    void step(DoorTask *task, SchedulerContext *context) override;
  };

public:
  DoorTask(ServoMotor *servo, int closedAngle, int openAngle);
};
