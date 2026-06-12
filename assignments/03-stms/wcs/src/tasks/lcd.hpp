#pragma once

#include "task.hpp"
#include "core/mode.hpp"
#include "devices/lcd.hpp"

class LCDTask : public AsyncTask
{
private:
    LCD *lcd;
    
    void prepareRow(uint8_t row);
    void displayMode(OperationMode mode);
    void displayOpeningLevel(uint8_t openingPercentage);

    class OperationModeObserver : public TaskEventObserver<LCDTask, OperationModeEvent>
    {
    public:
        OperationModeObserver(LCDTask *task, EventFamily family)
            : TaskEventObserver(task, family) {}
        void onEvent(OperationModeEvent event) override;
    };
    OperationModeObserver operationModeObserver;

public:
    LCDTask(LCD *lcd, EventFamily operationModeEventFamily);
    void begin(EventsManager *eventsManager) override;
};
