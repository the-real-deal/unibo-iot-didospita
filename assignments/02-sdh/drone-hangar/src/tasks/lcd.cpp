#include "lcd.hpp"
#include "blocking.hpp"
#include "core/utils.hpp"

LCDTask::LCDTask(LiquidCrystal_I2C *lcd)
    : Task<LCDTask>(new LCDTask::PrintStateAction()), lcd(lcd) {
  this->lcd->init();
  this->lcd->backlight();
  this->lcd->clear();
}

void LCDTask::PrintStateAction::step(LCDTask *task, uint64_t elapsedTime,
                                     StateManager *stateManager) {
  task->lcd->clear();
  task->lcd->setCursor(0, 0);
  String message = enumToString(stateManager->getState(), STATE_TYPE_STRINGS);
  task->lcd->println(message);
  task->switchAction(new LCDTask::IdleAction());
}

void LCDTask::IdleAction::step(LCDTask *task, uint64_t elapsedTime,
                               StateManager *stateManager) {
  blockOnAlarm(task, stateManager, new LCDTask::IdleAction());
  if (stateManager->hasPreviousState() &&
      stateManager->getPreviousState() != stateManager->getState()) {
    task->switchAction(new LCDTask::PrintStateAction());
  }
}
