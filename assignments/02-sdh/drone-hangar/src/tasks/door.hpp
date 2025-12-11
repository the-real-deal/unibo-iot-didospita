#pragma once

#include "core/tasks.hpp"
#include "io/servo.hpp"

class DoorTask : public Task<DoorTask> {
private:
  ServoMotor *servo;
  int closedAngle;
  int openAngle;

  class OpenState : public TaskState<DoorTask> {
  public:
    void step(DoorTask *task, SchedulerContext *context) override;
  };

  class ClosedState : public TaskState<DoorTask> {
  public:
    void step(DoorTask *task, SchedulerContext *context) override;
  };

public:
  DoorTask(ServoMotor *servo, int closedAngle, int openAngle);
};
