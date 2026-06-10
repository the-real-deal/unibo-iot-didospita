#pragma once

#include <Arduino.h>

#include "setup.hpp"
#include "message.hpp"

#ifndef ARDUINO_SERIAL_BAUD
#define ARDUINO_SERIAL_BAUD 9600
#endif

#ifndef SERIAL_MESSAGE_DELIMITER
#define SERIAL_MESSAGE_DELIMITER ':'
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
