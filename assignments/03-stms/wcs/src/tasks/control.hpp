#pragma once

#include "task.hpp"
#include "core/system.hpp"
#include "devices/potentiometer.hpp"
#include "devices/servo.hpp"
#include "devices/serial.hpp"

class ControlTask : public AsyncTask
{
private:
    ServoMotor *servo;

    class SystemStateObserver : public TaskEventObserver<ControlTask, SystemStatusChangeEvent>
    {
    public:
        SystemStateObserver(ControlTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(SystemStatusChangeEvent event) override;
    };
    SystemStateObserver systemStateObserver;

    class PotentiometerObserver : public TaskEventObserver<ControlTask, PotentiometerEvent>
    {
    public:
        PotentiometerObserver(ControlTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(PotentiometerEvent event) override;
    };
    PotentiometerObserver potentiometerObserver;

    class SerialObserver : public TaskEventObserver<ControlTask, SerialMessage>
    {
    public:
        SerialObserver(ControlTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(SerialMessage event) override;
    };
    SerialObserver serialObserver;

public:
    ControlTask(ServoMotor *servo,
                EventFamily statusChangeEventFamily,
                EventFamily potEventFamily,
                EventFamily serialEventFamily);
    void begin(EventsManager *eventsManager) override;
};