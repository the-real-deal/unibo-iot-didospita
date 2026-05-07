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
  uint32_t outsideTimeMillis;
  float insideDistanceMm;
  uint32_t insideTimeMillis;
  Timer timer;

  void sendDistance(float distance);

  class IdleState : public TaskState<DDDTask>
  {
  public:
    void step(DDDTask *task, Context *context) override;
  };
  static IdleState IDLE;
  static BlockedTaskState<DDDTask> BLOCKED_IDLE;

  class TakeoffReadingState : public TaskState<DDDTask>
  {
  public:
    void step(DDDTask *task, Context *context) override;
  };
  static TakeoffReadingState TAKEOFF_READING;

  class TakeoffDistanceCheckingState : public TaskState<DDDTask>
  {
  public:
    void setup(DDDTask *task) override;
    void step(DDDTask *task, Context *context) override;
  };
  static TakeoffDistanceCheckingState TAKEOFF_DISTANCE_CHECKING;

  class LandingReadingState : public TaskState<DDDTask>
  {
  public:
    void step(DDDTask *task, Context *context) override;
  };
  static LandingReadingState LANDING_READING;

  class LandingDistanceCheckingState : public TaskState<DDDTask>
  {
  public:
    void setup(DDDTask *task) override;
    void step(DDDTask *task, Context *context) override;
  };
  static LandingDistanceCheckingState LANDING_DISTANCE_CHECKING;

public:
  DDDTask(DistanceSensor *droneDistanceSensor, MessageService *messageService,
          float outsideDistance, uint32_t outsideTimeMillis,
          float insideDistance, uint32_t insideTimeMillis);
};
