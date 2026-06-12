#include "mode.hpp"

void OperationModeTask::ButtonObserver::onEvent(ButtonEvent event)
{
    Serial.print(F("MODE BTN "));
    switch (event)
    {
    case ButtonEvent::Press:
      Serial.println(F("PRESS"));
      break;

    case ButtonEvent::Release:
      Serial.println(F("RELEASE"));
      break;
    }
    Serial.flush();
}

OperationModeTask::OperationModeTask(EventSource<ButtonEvent> *button)
    : buttonObserver(this, button->getFamily()) {}

void OperationModeTask::OperationModeTask::begin(EventsManager *eventsManager) {
    eventsManager->registerObserver(&this->buttonObserver);
}