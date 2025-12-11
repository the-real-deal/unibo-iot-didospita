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
    void step(DoorTask *task, uint64_t elapsedTime,
              StateManager *stateManager) override;
  };

  class CloseAction : public TaskAction<DoorTask> {
  public:
    void step(DoorTask *task, uint64_t elapsedTime,
              StateManager *stateManager) override;
  };

public:
  DoorTask(ServoMotor *servo, int closedAngle, int openAngle);
};
