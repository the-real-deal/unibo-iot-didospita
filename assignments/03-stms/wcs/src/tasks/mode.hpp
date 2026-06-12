#pragma once

#include "task.hpp"
#include "kernel/events.hpp"
#include "core/mode.hpp"
#include "devices/button.hpp"
#include "devices/serial.hpp"

class OperationModeTask : public AsyncTask, public EventSource<OperationModeEvent>
{
private:
    OperationMode mode;

    bool generateModeEvent(OperationMode mode);
    void toggleMode();
    void switchMode(OperationMode mode);

    class ButtonObserver : public TaskEventObserver<OperationModeTask, ButtonEvent>
    {
    public:
        ButtonObserver(OperationModeTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(ButtonEvent event) override;
    };
    ButtonObserver buttonObserver;

    class SerialObserver : public TaskEventObserver<OperationModeTask, SerialMessage>
    {
    public:
        SerialObserver(OperationModeTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(SerialMessage event) override;
    };
    SerialObserver serialObserver;

public:
    OperationModeTask(OperationMode initialMode,
                      EventFamily operationsModeEventFamily,
                      EventFamily buttonEventFamily,
                      EventFamily serialEventFamily);
    void begin(EventsManager *eventsManager) override;
};