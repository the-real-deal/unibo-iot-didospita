#pragma once

#include "task.hpp"
#include "kernel/events.hpp"
#include "core/system.hpp"
#include "devices/button.hpp"
#include "devices/serial.hpp"

class SystemStatusTask : public AsyncTask, public EventSource<SystemStatusChangeEvent>
{
private:
    SystemStatus status;

    bool generateStatusEvent(SystemStatus status);
    void switchStatus(SystemStatus status);

    class ButtonObserver : public TaskEventObserver<SystemStatusTask, ButtonEvent>
    {
    public:
        ButtonObserver(SystemStatusTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(ButtonEvent event) override;
    };
    ButtonObserver buttonObserver;

    class SerialObserver : public TaskEventObserver<SystemStatusTask, SerialMessage>
    {
    public:
        SerialObserver(SystemStatusTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(SerialMessage event) override;
    };
    SerialObserver serialObserver;

public:
    SystemStatusTask(SystemStatus initialState,
                      EventFamily operationsModeEventFamily,
                      EventFamily buttonEventFamily,
                      EventFamily serialEventFamily);
    void begin(EventsManager *eventsManager) override;
};