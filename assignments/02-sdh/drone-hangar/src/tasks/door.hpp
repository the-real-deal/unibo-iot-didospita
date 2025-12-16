#pragma once

#include "core/tasks.hpp"
#include "io/message.hpp"
#include "io/servo.hpp"

class DoorTask : public Task<DoorTask> {
 private:
  ServoMotor* servo;
  int closedAngle;
  int openAngle;
  MessageService* messageService;

  class OpenState : public TaskState<DoorTask> {
   public:
    void step(DoorTask* task, Context* context) override;
  };

  class ClosedState : public TaskState<DoorTask> {
   public:
    void step(DoorTask* task, Context* context) override;
  };

 public:
  DoorTask(ServoMotor* servo, int closedAngle, int openAngle,
           MessageService* messageService);
};
