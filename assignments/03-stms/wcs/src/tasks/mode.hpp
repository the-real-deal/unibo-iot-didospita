#pragma once

#include "task.hpp"
#include "devices/button.hpp"

class OperationModeTask : public AsyncTask
{
private:
    class ButtonObserver : public TaskEventObserver<OperationModeTask, ButtonEvent>
    {
    public:
        ButtonObserver(OperationModeTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(ButtonEvent event) override;
    };
    ButtonObserver buttonObserver;

public:
    OperationModeTask(EventSource<ButtonEvent> *button);
    void begin(EventsManager *eventsManager) override;
};