#pragma once

#include "task.hpp"
#include "kernel/events.hpp"
#include "core/system.hpp"
#include "devices/button.hpp"
#include "devices/serial.hpp"

class SystemStateTask : public EventSource<SystemState>
{
private:
    SystemState state;

    bool generateStateEvent(SystemState state);
    void switchState(SystemState state);

    class ButtonObserver : public TaskEventObserver<SystemStateTask, ButtonEvent>
    {
    public:
        ButtonObserver(SystemStateTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(ButtonEvent event) override;
    };
    ButtonObserver buttonObserver;

    class SerialObserver : public TaskEventObserver<SystemStateTask, SerialMessage>
    {
    public:
        SerialObserver(SystemStateTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(SerialMessage event) override;
    };
    SerialObserver serialObserver;

public:
    SystemStateTask(SystemState initialState,
                      EventFamily systemStateEventFamily,
                      EventFamily buttonEventFamily,
                      EventFamily serialEventFamily);
    void begin(EventsManager *eventsManager) override;
};