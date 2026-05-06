#pragma once

#include "blocking.hpp"
#include "core/tasks.hpp"
#include "io/message.hpp"
#include "io/servo.hpp"

class DoorTask : public Task<DoorTask>
{
private:
  ServoMotor *servo;
  int closedAngle;
  int openAngle;
  int angleMargin;
  MessageService *messageService;

  class ClosedState : public TaskState<DoorTask>
  {
  public:
    void step(DoorTask *task, Context *context) override;
  };
  ClosedState closedState;
  BlockedTaskState<DoorTask> blockedClosedState;

  class OpenState : public TaskState<DoorTask>
  {
  public:
    void step(DoorTask *task, Context *context) override;
  };
  OpenState openState;

public:
  DoorTask(ServoMotor *servo, int closedAngle, int openAngle,
           int angleMargin, MessageService *messageService);
};
