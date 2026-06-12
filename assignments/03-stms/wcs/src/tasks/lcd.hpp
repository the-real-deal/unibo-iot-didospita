#pragma once

#include "task.hpp"
#include "core/system.hpp"
#include "devices/lcd.hpp"

class LCDTask : public AsyncTask
{
private:
    LCD *lcd;
    
    void prepareRow(uint8_t row);
    void displayStatus(SystemStatus status);
    void displayOpeningLevel(uint8_t openingPercentage);

    class SystemModeObserver : public TaskEventObserver<LCDTask, SystemStatusChangeEvent>
    {
    public:
        SystemModeObserver(LCDTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(SystemStatusChangeEvent event) override;
    };
    SystemModeObserver systemModeObserver;

public:
    LCDTask(LCD *lcd, EventFamily statusChangeEventFamily);
    void begin(EventsManager *eventsManager) override;
};
