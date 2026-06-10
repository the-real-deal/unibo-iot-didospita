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

template <typename Type>
class EventSignal
{
private:
    Type type;

public:
    EventSignal(Type type) : type(type) {}
    Type getType() { return this->type; }
};

template <typename Type, typename Data>
class Event : public EventSignal<Type>
{
private:
    Data data;

public:
    Event(Type type, Data data) : EventSignal<Type>(type), data(data) {}
    Data getData() { return this->data; }
};

template <typename Type>
class EventObserver
{
public:
    virtual void onEvent(EventSignal<Type> *event) = 0;
    ~EventObserver() = default;
};

template <typename Type>
class EventManager
{
private:
    Array<EventSignal<Type> *, EVENT_QUEUE_SIZE> eventQueue;
    Array<EventObserver<Type> *, MAX_EVENT_OBSERVERS> observers;

public:
    EventManager() : eventQueue(), observers() {};

    bool registerObserver(EventObserver<Type> *observer)
    {
        return this->observers.pushLast(observer);
    }

    bool generateEvent(EventSignal<Type> *event)
    {
        return this->eventQueue.pushLast(event);
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
        EventSignal<Type> *event = *this->eventQueue.get(0);
        interrupts();

        for (size_t i = 0; i < this->observers.size(); i++)
        {
            EventObserver<Type> *observer = *this->observers.get(i);
            observer->onEvent(event);
        }

        delete event;
        noInterrupts();
        this->eventQueue.deleteAt(0);
        interrupts();
    }
};

template <typename Type>
class EventSource
{
private:
    EventManager<Type> *eventManager;

protected:
    bool generateEvent(EventSignal<Type> *event)
    {
        if (this->eventManager != nullptr)
        {
            return this->eventManager->generateEvent(event);
        }
        return false;
    }

public:
    EventSource() : eventManager() {}

    void attachToEventManager(EventManager<Type> *eventManager)
    {
        this->eventManager = eventManager;
    }
};
