#include "lcd.hpp"
#include "blocking.hpp"
#include "utils.hpp"

LCDTask::LCDTask(LiquidCrystal_I2C *lcd)
    : Task<LCDTask>(new LCDTask::PrintState()), lcd(lcd) {
  this->lcd->init();
  this->lcd->backlight();
  this->lcd->clear();
}

void LCDTask::IdleState::step(LCDTask *task, SchedulerContext *context) {
  blockOnAlarm<LCDTask, LCDTask::IdleState>(task, context);
  GlobalState state = context->getState();
  switch (state) {
  case GlobalState::RequestLanding:
    break;
  default:
    if (context->getPreviousState() != state) {
      task->switchState(new LCDTask::PrintState());
    }
    break;
  }
}

void LCDTask::PrintState::step(LCDTask *task, SchedulerContext *context) {
  task->lcd->clear();
  task->lcd->setCursor(0, 0);
  String message = enumToString(context->getState(), GLOBAL_STATE_STRINGS);
  task->lcd->println(message);
  task->switchState(new LCDTask::IdleState());
}
