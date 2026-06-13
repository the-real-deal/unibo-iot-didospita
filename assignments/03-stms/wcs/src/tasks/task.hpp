#pragma once

#include "kernel/setup.hpp"
#include "kernel/events.hpp"

template <typename Task, typename T>
class TaskEventObserver : public EventObserver<T>
{
protected:
    Task *task;

public:
    TaskEventObserver(Task *task, EventFamily family)
        : EventObserver<T>(family), task(task) {}
};