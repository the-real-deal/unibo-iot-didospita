#pragma once

#include "blocking.hpp"
#include "core/tasks.hpp"
#include "io/message.hpp"
#include "io/presence.hpp"

class DPDTask : public Task<DPDTask>
{
private:
  PresenceSensor *dronePresenceSensor;
  MessageService *messageService;

  class IdleState : public TaskState<DPDTask>
  {
  public:
    void step(DPDTask *task, Context *context) override;
  };
  IdleState idleState;
  BlockedTaskState<DPDTask> blockedIdleState;

  class ReadingState : public TaskState<DPDTask>
  {
  public:
    void step(DPDTask *task, Context *context) override;
  };
  ReadingState readingState;

public:
  DPDTask(PresenceSensor *dronePresenceSensor, MessageService *messageService);
};
