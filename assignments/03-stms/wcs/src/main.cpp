#include <Arduino.h>

#include "core/serial.hpp"
#include "devices/button.hpp"

SerialManager serialManager;
PushButton button(2);

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
  delay(1000);
}