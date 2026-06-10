#pragma once

#include <Arduino.h>

#include "setup.hpp"
#include "message.hpp"

#ifndef ARDUINO_SERIAL_BAUD
#define ARDUINO_SERIAL_BAUD 9600
#endif

class SerialManager : public Setup
{
private:
  void sendMessage(SerialMessageType type, const char *message);
public:
  SerialManager();
  void setup() override;
  void serialSync();
  void log(const char *message);
};
