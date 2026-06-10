#include <Arduino.h>

#include "core/serial.hpp"
#include "devices/button.hpp"

SerialManager serialManager;
PushButton button(2);

void setup()
{
  serialManager.setup();  
  serialManager.log("setup() started");
  button.setup();
  serialManager.log("setup() finished");
}

void loop()
{
  serialManager.log("loop() is running");
  delay(1000);
}