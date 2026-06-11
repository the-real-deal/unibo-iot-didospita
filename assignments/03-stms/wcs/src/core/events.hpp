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

class EventsManager;

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

class EventSignalObserver
{
    friend EventsManager;

protected:
    EventFamily family;

    virtual void onEventSignal(EventSignal *event) = 0;

public:
    EventSignalObserver(EventFamily family) : family(family) {}
};

template <typename T>
class EventObserver : public EventSignalObserver
{
protected:
    void onEventSignal(EventSignal *event) override
    {
        T eventData = static_cast<Event<T> *>(event)->getData();
        this->onEvent(eventData);
    }

    virtual void onEvent(T eventData) = 0;

public:
    EventObserver(EventFamily family) : EventSignalObserver(family) {}
};

class EventsManager
{
private:
    Array<EventSignal *, EVENT_QUEUE_SIZE> eventQueue;
    Array<EventSignalObserver *, MAX_EVENT_OBSERVERS> observers;

public:
    EventsManager() : eventQueue(), observers() {};

    bool registerObserver(EventSignalObserver *observer)
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
            EventSignalObserver *observer = *this->observers.get(i);
            if (observer->family == event->getFamily())
            {
                observer->onEventSignal(event);
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
    EventsManager *eventManager;

    bool generateEvent(T eventData)
    {
        if (this->eventManager != nullptr)
        {
            return this->eventManager->generateEvent(new Event<T>(this->family, eventData));
        }
        return false;
    }

public:
    EventSource(EventFamily family, EventsManager *eventManager)
        : family(family), eventManager(eventManager) {}
};

template <typename T>
class SyncEventSource : public EventSource<T>
{
public:
    SyncEventSource(EventFamily family, EventsManager *eventManager)
        : EventSource<T>(family, eventManager) {}

    virtual void checkEvents() = 0;
};
