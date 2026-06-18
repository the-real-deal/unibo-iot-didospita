#pragma once

#include "task.hpp"
#include "kernel/events.hpp"
#include "core/system.hpp"
#include "devices/potentiometer.hpp"
#include "devices/servo.hpp"
#include "devices/serial.hpp"

class ControlTask : public EventActor
{
private:
    ServoMotor *servo;
    SerialManager *serialManager;

    class SystemStateObserver : public TaskEventObserver<ControlTask, SystemState>
    {
    public:
        SystemStateObserver(ControlTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(SystemState state) override;
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
                SerialManager *serialManager,
                EventFamily systemStateEventFamily,
                EventFamily potEventFamily,
                EventFamily serialEventFamily);
    void begin(EventsManager *eventsManager) override;
};