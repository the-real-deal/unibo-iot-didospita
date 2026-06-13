#include "events.hpp"

void EventSignalObserver::begin(EventsManager *eventsManager)
{
    eventsManager->registerObserver(this);
}
