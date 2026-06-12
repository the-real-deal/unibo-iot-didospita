#pragma once

#include "task.hpp"
#include "core/system.hpp"
#include "devices/lcd.hpp"
#include "devices/serial.hpp"
#include "devices/servo.hpp"

class DisplayTask : public AsyncTask
{
private:
    LCD *lcd;
    SerialManager *serialManager;

    void displayStatus(SystemStatus status);
    void displayAngle(int angle);

    class SystemStateObserver : public TaskEventObserver<DisplayTask, SystemStatusChangeEvent>
    {
    public:
        SystemStateObserver(DisplayTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(SystemStatusChangeEvent event) override;
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
    DisplayTask(LCD *lcd, SerialManager *serialManager,
                EventFamily statusChangeEventFamily, EventFamily servoEventFamily);
    void begin(EventsManager *eventsManager) override;
};
