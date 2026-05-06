#pragma once

#include "blocking.hpp"
#include "core/tasks.hpp"
#include "io/indicator.hpp"
#include "io/message.hpp"
#include "io/temperature.hpp"

class AlarmTask : public Task<AlarmTask>
{
private:
  TemperatureSensor *hangarTempSensor;
  Indicator *alarmIndicator;
  GlobalState prevState;
  float prealarmTemp;
  uint64_t prealarmTimeMillis;
  float alarmTemp;
  uint64_t alarmTimeMillis;

  class IdleState : public TaskState<AlarmTask>
  {
  public:
    void step(AlarmTask *task, Context *context) override;
  };
  IdleState idleState;
  BlockedTaskState<AlarmTask> blockIdleState;

  class PrealarmReadingState : public TaskState<AlarmTask>
  {
  public:
    void step(AlarmTask *task, Context *context) override;
  };
  PrealarmReadingState prealarmReadingState;

  class PrealarmTempCheckingState : public TaskState<AlarmTask>
  {
  private:
    Timer timer;

  public:
    PrealarmTempCheckingState(AlarmTask *task);
    void setup(AlarmTask *task) override;
    void step(AlarmTask *task, Context *context) override;
  };
  PrealarmTempCheckingState prealarmTempCheckingState;

  class WaitOperationsState : public TaskState<AlarmTask>
  {
  public:
    void step(AlarmTask *task, Context *context) override;
  };
  WaitOperationsState waitOperationsState;

  class AlarmReadingState : public TaskState<AlarmTask>
  {
  public:
    void step(AlarmTask *task, Context *context) override;
  };
  AlarmReadingState alarmReadingState;

  class AlarmTempCheckingState : public TaskState<AlarmTask>
  {
  private:
    Timer timer;

  public:
    AlarmTempCheckingState(AlarmTask *task);
    void setup(AlarmTask *task) override;
    void step(AlarmTask *task, Context *context) override;
  };
  AlarmTempCheckingState alarmTempCheckingState;

public:
  AlarmTask(TemperatureSensor *hangarTempSensor, Indicator *alarmIndicator,
            GlobalState initialState, float prealarmTemp,
            uint64_t prealarmTimeMillis, float alarmTemp,
            uint64_t alarmTimeMillis);
};
