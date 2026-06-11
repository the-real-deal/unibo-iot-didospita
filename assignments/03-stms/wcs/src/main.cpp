#include <Arduino.h>

#include "core/serial.hpp"
#include "devices/button.hpp"

#ifndef BTN_EVENT_FAMILY
#define BTN_EVENT_FAMILY 0
#endif

class BtnLogObserver: public EventObserver<ButtonEvent> {
protected:
  void onEvent(ButtonEvent eventData) override {
    Serial.print(F("BTN "));
    switch (eventData)
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

public:
  BtnLogObserver() : EventObserver(BTN_EVENT_FAMILY) {}
};

SerialManager serialManager;
EventManager eventManager;
PushButton button(2, BTN_EVENT_FAMILY, &eventManager);

BtnLogObserver btnObserver;

void setup()
{
  serialManager.setup();
  serialManager.log("setup() started");

  eventManager.registerObserver(&btnObserver);
  button.setup();

  serialManager.log("setup() finished");
}

void loop()
{
  serialManager.log("loop() is running");
  eventManager.handleEvents();
  delay(200);
}