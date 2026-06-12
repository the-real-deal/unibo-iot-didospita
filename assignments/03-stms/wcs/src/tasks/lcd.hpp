#pragma once

#include "task.hpp"
#include "core/system.hpp"
#include "devices/lcd.hpp"
#include "devices/servo.hpp"

#ifndef POT_SERVO_MAX_ANGLE
#define POT_SERVO_MAX_ANGLE 90
#endif

class LCDTask : public AsyncTask
{
private:
    LCD *lcd;

    void prepareRow(uint8_t row);
    void displayStatus(SystemStatus status);
    void displayOpeningLevel(uint8_t openingPercentage);

    class SystemStateObserver : public TaskEventObserver<LCDTask, SystemStatusChangeEvent>
    {
    public:
        SystemStateObserver(LCDTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(SystemStatusChangeEvent event) override;
    };
    SystemStateObserver systemStateObserver;

    class ServoMotorObserver : public TaskEventObserver<LCDTask, ServoMotorEvent>
    {
    public:
        ServoMotorObserver(LCDTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(ServoMotorEvent event) override;
    };
    ServoMotorObserver servoObserver;

public:
    LCDTask(LCD *lcd, EventFamily statusChangeEventFamily, EventFamily servoEventFamily);
    void begin(EventsManager *eventsManager) override;
};
