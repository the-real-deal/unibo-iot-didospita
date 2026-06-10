#include <Arduino.h>

#include "core/serial.hpp"

SerialManager serialManager;

void setup()
{
  serialManager.setup();

  serialManager.log("setup() is running");
}

void loop()
{
  serialManager.log("loop() is running");
  delay(1000);
}