#include "lcd.hpp"

#include "std/enum.hpp"

void LCDTask::prepareRow(uint8_t row)
{
    this->lcd->clearRow(row);
    this->lcd->setCursor(0, row);
}

void LCDTask::displayMode(OperationMode mode)
{
    this->prepareRow(0);
    this->lcd->print(enumToString(mode, OPERATION_MODE_STRINGS));
}

void LCDTask::OperationModeObserver::onEvent(OperationModeEvent event)
{
    this->task->displayMode(event.mode);
}

LCDTask::LCDTask(LCD *lcd, EventFamily operationModeEventFamily)
    : lcd(lcd), operationModeObserver(this, operationModeEventFamily) {}

void LCDTask::begin(EventsManager *eventsManager)
{
    eventsManager->registerObserver(&this->operationModeObserver);
}