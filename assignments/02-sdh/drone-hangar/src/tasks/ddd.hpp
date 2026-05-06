#pragma once

#include "blocking.hpp"
#include "core/tasks.hpp"
#include "io/distance.hpp"
#include "io/message.hpp"

class DDDTask : public Task<DDDTask>
{
private:
  DistanceSensor *droneDistanceSensor;
  MessageService *messageService;
  float outsideDistanceMm;
  uint64_t outsideTimeMillis;
  float insideDistanceMm;
  uint64_t insideTimeMillis;

  void sendDistance(float distance);

  class IdleState : public TaskState<DDDTask>
  {
  public:
    void step(DDDTask *task, Context *context) override;
  };
  IdleState idleState;
  BlockedTaskState<DDDTask> blockedIdleState;

  class TakeoffReadingState : public TaskState<DDDTask>
  {
  public:
    void step(DDDTask *task, Context *context) override;
  };
  TakeoffReadingState takeoffReadingState;

  class TakeoffDistanceCheckingState : public TaskState<DDDTask>
  {
  private:
    Timer timer;

  public:
    TakeoffDistanceCheckingState(DDDTask *task);
    void setup(DDDTask *task) override;
    void step(DDDTask *task, Context *context) override;
  };
  TakeoffDistanceCheckingState takeoffDistanceCheckingState;

  class LandingReadingState : public TaskState<DDDTask>
  {
  public:
    void step(DDDTask *task, Context *context) override;
  };
  LandingReadingState landingReadingState;

  class LandingDistanceCheckingState : public TaskState<DDDTask>
  {
  private:
    Timer timer;

  public:
    LandingDistanceCheckingState(DDDTask *task);
    void setup(DDDTask *task) override;
    void step(DDDTask *task, Context *context) override;
  };
  LandingDistanceCheckingState landingDistanceCheckingState;

public:
  DDDTask(DistanceSensor *droneDistanceSensor, MessageService *messageService,
          float outsideDistance, uint64_t outsideTimeMillis,
          float insideDistance, uint64_t insideTimeMillis);
};
