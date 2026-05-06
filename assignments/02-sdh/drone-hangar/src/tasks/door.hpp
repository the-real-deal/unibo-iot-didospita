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
  static ClosedState CLOSED;
  static BlockedTaskState<DoorTask> BLOCKED_CLOSED;

  class OpenState : public TaskState<DoorTask>
  {
  public:
    void step(DoorTask *task, Context *context) override;
  };
  static OpenState OPEN;

public:
  DoorTask(ServoMotor *servo, int closedAngle, int openAngle,
           int angleMargin, MessageService *messageService);
};
