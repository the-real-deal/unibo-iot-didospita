#include "display.hpp"

#include "config.h"
#include "std/enum.hpp"
#include "std/percentage.hpp"

void DisplayTask::displayState(SystemState state)
{
    this->lcd->clearRow(0);
    this->lcd->print(enumToString(state, SYSTEM_STATE_STRINGS));
}

void DisplayTask::displayAngle(int angle)
{
    float percentage = toPercentage(angle, POT_SERVO_MAX_ANGLE);
    uint8_t displayPercentage = round(percentage * 100.0);
    String displayString = String((uint16_t)displayPercentage) + "%";
    this->lcd->clearRow(1);
    this->lcd->print(displayString.c_str());
}

void DisplayTask::SystemStateObserver::onEvent(SystemState state)
{
    this->task->displayState(state);
}

void DisplayTask::ServoMotorObserver::onEvent(ServoMotorEvent event)
{
    this->task->displayAngle(event.angle);
}

DisplayTask::DisplayTask(LCD *lcd, EventFamily systemStateEventFamily, EventFamily servoEventFamily)
    : lcd(lcd),
      systemStateObserver(this, systemStateEventFamily),
      servoObserver(this, servoEventFamily) {}

void DisplayTask::begin(EventsManager *eventsManager)
{
    this->systemStateObserver.begin(eventsManager);
    this->servoObserver.begin(eventsManager);
}