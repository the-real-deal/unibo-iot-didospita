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
  Timer timer;

  class IdleState : public TaskState<DPDTask>
  {
  public:
    void step(DPDTask *task, Context *context) override;
  };
  static IdleState IDLE;
  static BlockedTaskState<DPDTask> BLOCKED_IDLE;

  class ReadingState : public TaskState<DPDTask>
  {
  public:
    void setup(DPDTask *task) override;
    void step(DPDTask *task, Context *context) override;
  };
  static ReadingState READING;

public:
  DPDTask(PresenceSensor *dronePresenceSensor, MessageService *messageService);
};
