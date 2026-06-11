#include <Arduino.h>

#include "core/serial.hpp"
#include "devices/button.hpp"

#ifndef BTN_EVENT_FAMILY
#define BTN_EVENT_FAMILY 0
#endif

SerialManager serialManager;
EventManager eventManager;
PushButton button(2, BTN_EVENT_FAMILY, &eventManager);

void logPress(PushButton *btn)
{
  Serial.print(F("Button press at pin "));
  Serial.println(btn->getPin());
  Serial.flush();
}

void logRelease(PushButton *btn)
{
  Serial.print(F("Button release at pin "));
  Serial.println(btn->getPin());
  Serial.flush();
}

void setup()
{
  serialManager.setup();
  serialManager.log("setup() started");

  button.onPress(logPress);
  button.onRelease(logRelease);
  button.setup();

  serialManager.log("setup() finished");
}

void loop()
{
  serialManager.log("loop() is running");
  eventManager.handleEvents();
  delay(200);
}