#include "lcd.hpp"
#include "blocking.hpp"
#include "utils.hpp"

LCDTask::LCDTask(LiquidCrystal_I2C *lcd)
    : Task<LCDTask>(new LCDTask::PrintState()), lcd(lcd) {
  this->lcd->init();
  this->lcd->backlight();
  this->lcd->clear();
}

void LCDTask::PrintState::step(LCDTask *task, SchedulerContext *context) {
  task->lcd->clear();
  task->lcd->setCursor(0, 0);
  String message = enumToString(context->getState(), STATE_TYPE_STRINGS);
  task->lcd->println(message);
  task->switchTo(new LCDTask::IdleState());
}

void LCDTask::IdleState::step(LCDTask *task, SchedulerContext *context) {
  blockOnAlarm(task, context, new LCDTask::IdleState());
  if (context->hasPreviousState() &&
      context->getPreviousState() != context->getState()) {
    task->switchTo(new LCDTask::PrintState());
  }
}
