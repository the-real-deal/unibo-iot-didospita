#include "display.hpp"

#include "config.h"
#include "std/enum.hpp"

void DisplayTask::displayStatus(SystemStatus status)
{
    this->serialManager->sendStatus(status);
    this->lcd->clearRow(0);
    this->lcd->print(enumToString(status, SYSTEM_STATUS_STRINGS));
}

void DisplayTask::displayAngle(int angle)
{
    this->serialManager->sendAngle(angle);

    uint8_t openingPercentage = min(((double)angle / (double)POT_SERVO_MAX_ANGLE), 1.0) * 100;
    String displayString = String((uint16_t)openingPercentage) + "%";
    this->lcd->clearRow(1);
    this->lcd->print(displayString.c_str());
}

void DisplayTask::SystemStateObserver::onEvent(SystemStatusChangeEvent event)
{
    this->task->displayStatus(event.status);
}

void DisplayTask::ServoMotorObserver::onEvent(ServoMotorEvent event)
{
    this->task->displayAngle(event.angle);
}

DisplayTask::DisplayTask(LCD *lcd, SerialManager *serialManager,
                         EventFamily statusChangeEventFamily, EventFamily servoEventFamily)
    : lcd(lcd), serialManager(serialManager),
      systemStateObserver(this, statusChangeEventFamily),
      servoObserver(this, servoEventFamily) {}

void DisplayTask::begin(EventsManager *eventsManager)
{
    eventsManager->registerObserver(&this->systemStateObserver);
    eventsManager->registerObserver(&this->servoObserver);
}