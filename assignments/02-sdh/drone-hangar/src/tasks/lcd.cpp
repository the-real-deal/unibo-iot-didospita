#include "lcd.hpp"
#include "blocking.hpp"
#include "utils.hpp"

LCDTask::LCDTask(LiquidCrystal_I2C *lcd)
    : Task<LCDTask>(new LCDTask::PrintStateAction()), lcd(lcd) {
  this->lcd->init();
  this->lcd->backlight();
  this->lcd->clear();
}

void LCDTask::PrintStateAction::step(LCDTask *task, SchedulerContext *context) {
  task->lcd->clear();
  task->lcd->setCursor(0, 0);
  String message = enumToString(context->state(), STATE_TYPE_STRINGS);
  task->lcd->println(message);
  task->switchAction(new LCDTask::IdleAction());
}

void LCDTask::IdleAction::step(LCDTask *task, SchedulerContext *context) {
  blockOnAlarm(task, context, new LCDTask::IdleAction());
  if (context->hasPreviousState() &&
      context->previousState() != context->state()) {
    task->switchAction(new LCDTask::PrintStateAction());
  }
}
