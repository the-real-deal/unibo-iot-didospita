#pragma once

#include "task.hpp"
#include "kernel/events.hpp"
#include "core/system.hpp"
#include "devices/lcd.hpp"
#include "devices/servo.hpp"

class DisplayTask : public EventActor
{
private:
    LCD *lcd;

    void displayState(SystemState state);
    void displayAngle(int angle);

    class SystemStateObserver : public TaskEventObserver<DisplayTask, SystemState>
    {
    public:
        SystemStateObserver(DisplayTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(SystemState state) override;
    };
    SystemStateObserver systemStateObserver;

    class ServoMotorObserver : public TaskEventObserver<DisplayTask, ServoMotorEvent>
    {
    public:
        ServoMotorObserver(DisplayTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(ServoMotorEvent event) override;
    };
    ServoMotorObserver servoObserver;

public:
    DisplayTask(LCD *lcd, EventFamily systemStateEventFamily, EventFamily servoEventFamily);
    void begin(EventsManager *eventsManager) override;
};
