#include "lcd.hpp"

#include "std/enum.hpp"

void LCDTask::prepareRow(uint8_t row)
{
    this->lcd->clearRow(row);
    this->lcd->setCursor(0, row);
}

void LCDTask::displayStatus(SystemStatus status)
{
    this->prepareRow(0);
    this->lcd->print(enumToString(status, SYSTEM_STATUS_STRINGS));
}

void LCDTask::SystemModeObserver::onEvent(SystemStatusChangeEvent event)
{
    this->task->displayStatus(event.status);
}

LCDTask::LCDTask(LCD *lcd, EventFamily statusChangeEventFamily)
    : lcd(lcd), systemModeObserver(this, statusChangeEventFamily) {}

void LCDTask::begin(EventsManager *eventsManager)
{
    eventsManager->registerObserver(&this->systemModeObserver);
}