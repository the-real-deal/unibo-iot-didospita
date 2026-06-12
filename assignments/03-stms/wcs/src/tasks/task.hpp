#pragma once

#include "kernel/setup.hpp"
#include "kernel/events.hpp"

class AsyncTask
{
public:
    AsyncTask() {}
    virtual void begin(EventsManager *eventsManager);
};

template <typename Task, typename T>
class TaskEventObserver : public EventObserver<T>
{
protected:
    Task *task;

public:
    TaskEventObserver(Task *task, EventFamily family)
        : EventObserver<T>(family), task(task) {}
};