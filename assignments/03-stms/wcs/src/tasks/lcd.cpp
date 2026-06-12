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

void LCDTask::displayOpeningLevel(uint8_t openingPercentage)
{
    this->prepareRow(1);
    String displayString = String((uint16_t)openingPercentage) + "%";
    this->lcd->print(displayString.c_str());
}

void LCDTask::SystemStateObserver::onEvent(SystemStatusChangeEvent event)
{
    this->task->displayStatus(event.status);
}

void LCDTask::ServoMotorObserver::onEvent(ServoMotorEvent event)
{
    uint8_t openingLevel = min(((double)event.angle / (double)POT_SERVO_MAX_ANGLE), 1.0) * 100;
    this->task->displayOpeningLevel(openingLevel);
}

LCDTask::LCDTask(LCD *lcd,
                 EventFamily statusChangeEventFamily,
                 EventFamily servoEventFamily)
    : lcd(lcd),
      systemStateObserver(this, statusChangeEventFamily),
      servoObserver(this, servoEventFamily) {}

void LCDTask::begin(EventsManager *eventsManager)
{
    eventsManager->registerObserver(&this->systemStateObserver);
    eventsManager->registerObserver(&this->servoObserver);
}