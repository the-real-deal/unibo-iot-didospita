#include "alarm.hpp"

#include "blocking.hpp"
#include "utils.hpp"

AlarmTask::AlarmTask(TemperatureSensor* hangarTempSensor,
                     Indicator* alarmIndicator, GlobalState initialState,
                     float prealarmTemp, uint64_t prealarmTimeMillis,
                     float alarmTemp, uint64_t alarmTimeMillis)
    : Task<AlarmTask>(new IdleState()),
      hangarTempSensor(hangarTempSensor),
      alarmIndicator(alarmIndicator),
      prevState(initialState),
      prealarmTemp(prealarmTemp),
      prealarmTimeMillis(prealarmTimeMillis),
      alarmTemp(alarmTemp),
      alarmTimeMillis(alarmTimeMillis) {}

void AlarmTask::IdleState::step(AlarmTask* task, Context* context) {
  blockOnAlarm<AlarmTask, IdleState>(task, context);

  if (task->alarmIndicator->isOn()) {
    task->alarmIndicator->turnOff();
  }

  switch (context->getState()) {
    case GlobalState::Inside:
    case GlobalState::Takeoff:
    case GlobalState::Landing:
      task->switchState(new PrealarmReadingState());
      break;
    default:
      break;
  }
}

void AlarmTask::PrealarmReadingState::step(AlarmTask* task, Context* context) {
  switch (context->getState()) {
    case GlobalState::Inside:
    case GlobalState::Takeoff:
    case GlobalState::Landing:
      if (task->hangarTempSensor->getTemperature() >= task->prealarmTemp) {
        task->switchState(new PrealarmTempCheckingState(task));
      }
      break;
    default:
      task->switchState(new IdleState());
      break;
  }
}

AlarmTask::PrealarmTempCheckingState::PrealarmTempCheckingState(AlarmTask* task)
    : timer(task->prealarmTimeMillis) {}

void AlarmTask::PrealarmTempCheckingState::step(AlarmTask* task,
                                                Context* context) {
  if (this->timer.isFinished()) {
    task->switchState(new WaitOperationsState());
  } else if (task->hangarTempSensor->getTemperature() < task->prealarmTemp) {
    task->switchState(new PrealarmReadingState());
  }
}

void AlarmTask::WaitOperationsState::step(AlarmTask* task, Context* context) {
  GlobalState state = context->getState();
  switch (state) {
    case GlobalState::Takeoff:
    case GlobalState::Landing:
      break;
    default:
      task->prevState = state;
      context->setState(GlobalState::Prealarm);
      task->switchState(new AlarmReadingState());
      break;
  }
}

void AlarmTask::AlarmReadingState::step(AlarmTask* task, Context* context) {
  float temp = task->hangarTempSensor->getTemperature();
  if (temp >= task->alarmTemp) {
    task->switchState(new AlarmTempCheckingState(task));
  } else if (temp < task->prealarmTemp) {
    context->setState(task->prevState);
    task->switchState(new PrealarmReadingState());
  }
}

AlarmTask::AlarmTempCheckingState::AlarmTempCheckingState(AlarmTask* task)
    : timer(task->alarmTimeMillis) {}

void AlarmTask::AlarmTempCheckingState::step(AlarmTask* task,
                                             Context* context) {
  if (this->timer.isFinished()) {
    task->alarmIndicator->turnOn();

    context->setState(GlobalState::Alarm);
    task->switchState(new IdleState());
  } else if (task->hangarTempSensor->getTemperature() < task->alarmTemp) {
    task->switchState(new AlarmReadingState());
  }
}
