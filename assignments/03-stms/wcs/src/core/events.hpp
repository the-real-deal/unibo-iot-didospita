#pragma once

#include <stddef.h>
#include <Arduino.h>

#include "std/collections.hpp"

#ifndef EVENT_QUEUE_SIZE
#define EVENT_QUEUE_SIZE 10
#endif

#ifndef MAX_EVENT_OBSERVERS
#define MAX_EVENT_OBSERVERS 5
#endif

#ifndef MAX_EVENT_SOURCES
#define MAX_EVENT_SOURCES 5
#endif

using EventFamily = uint8_t;

class EventSignal
{
private:
    EventFamily family;

public:
    EventSignal(EventFamily family) : family(family) {}
    EventFamily getFamily() { return this->family; }
};

template <typename T>
class Event : public EventSignal
{
private:
    T data;

public:
    Event(EventFamily family, T data) : EventSignal(family), data(data) {}
    T getData() { return this->data; }
};

class EventObserver
{
private:
    EventFamily family;

public:
    EventObserver(EventFamily family) : family(family) {}

    virtual void onEvent(EventSignal *event) = 0;

    EventFamily getObservedEventFamily()
    {
        return this->family;
    }
};

class EventManager
{
private:
    Array<EventSignal *, EVENT_QUEUE_SIZE> eventQueue;
    Array<EventObserver *, MAX_EVENT_OBSERVERS> observers;

public:
    EventManager() : eventQueue(), observers() {};

    bool registerObserver(EventObserver *observer)
    {
        return this->observers.pushLast(observer);
    }

    bool generateEvent(EventSignal *event)
    {
        noInterrupts();
        if (this->eventQueue.isFull())
        {
            EventSignal *event = *this->eventQueue.get(0);
            delete event;
            this->eventQueue.deleteAt(0);
        }
        bool ok = this->eventQueue.pushLast(event);
        interrupts();
        return ok;
    }

    void handleEvents()
    {
        noInterrupts();
        bool noEvents = this->eventQueue.isEmpty();
        interrupts();

        if (noEvents)
        {
            return;
        }

        noInterrupts();
        EventSignal *event = *this->eventQueue.get(0);
        interrupts();

        for (size_t i = 0; i < this->observers.size(); i++)
        {
            EventObserver *observer = *this->observers.get(i);
            if (observer->getObservedEventFamily() == event->getFamily())
            {
                observer->onEvent(event);
            }
        }

        delete event;
        noInterrupts();
        this->eventQueue.deleteAt(0);
        interrupts();
    }
};

template <typename T>
class EventSource
{
protected:
    EventFamily family;
    EventManager *eventManager;

    bool generateEvent(T eventData)
    {
        if (this->eventManager != nullptr)
        {
            return this->eventManager->generateEvent(new Event<T>(this->family, eventData));
        }
        return false;
    }

public:
    EventSource(EventFamily family, EventManager *eventManager) 
        : family(family), eventManager(eventManager) {}
};
