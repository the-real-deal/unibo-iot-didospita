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
  Timer timer;

  class IdleState : public TaskState<AlarmTask>
  {
  public:
    void step(AlarmTask *task, Context *context) override;
  };
  static IdleState IDLE;
  static BlockedTaskState<AlarmTask> BLOCK_IDLE;

  class PrealarmReadingState : public TaskState<AlarmTask>
  {
  public:
    void step(AlarmTask *task, Context *context) override;
  };
  static PrealarmReadingState PREALARM_READING;

  class PrealarmTempCheckingState : public TaskState<AlarmTask>
  {
  public:
    void setup(AlarmTask *task) override;
    void step(AlarmTask *task, Context *context) override;
  };
  static PrealarmTempCheckingState PREALARM_TEMP_CHECKING;

  class WaitOperationsState : public TaskState<AlarmTask>
  {
  public:
    void step(AlarmTask *task, Context *context) override;
  };
  static WaitOperationsState WAIT_OPERATIONS;

  class AlarmReadingState : public TaskState<AlarmTask>
  {
  public:
    void step(AlarmTask *task, Context *context) override;
  };
  static AlarmReadingState ALARM_READING;

  class AlarmTempCheckingState : public TaskState<AlarmTask>
  {
  public:
    void setup(AlarmTask *task) override;
    void step(AlarmTask *task, Context *context) override;
  };
  static AlarmTempCheckingState ALARM_TEMP_CHECKING;

public:
  AlarmTask(TemperatureSensor *hangarTempSensor, Indicator *alarmIndicator, GlobalState initialState);
};
