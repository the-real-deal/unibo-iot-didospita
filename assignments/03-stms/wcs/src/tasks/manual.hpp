#pragma once

#include "task.hpp"
#include "core/system.hpp"
#include "devices/potentiometer.hpp"
#include "devices/servo.hpp"

class ManualControlTask : public AsyncTask
{
private:
    ServoMotor *servo;

    class SystemStateObserver : public TaskEventObserver<ManualControlTask, SystemStatusChangeEvent>
    {
    public:
        SystemStateObserver(ManualControlTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(SystemStatusChangeEvent event) override;
    };
    SystemStateObserver systemStateObserver;

    class PotentiometerObserver : public TaskEventObserver<ManualControlTask, PotentiometerEvent>
    {
    public:
        PotentiometerObserver(ManualControlTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(PotentiometerEvent event) override;
    };
    PotentiometerObserver potentiometerObserver;

public:
    ManualControlTask(ServoMotor *servo,
                      EventFamily statusChangeEventFamily,
                      EventFamily potEventFamily);
    void begin(EventsManager *eventsManager) override;
};