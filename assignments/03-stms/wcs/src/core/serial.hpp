#pragma once

#include <Arduino.h>

#include "setup.hpp"

#ifndef SERIAL_BAUD
#define SERIAL_BAUD 9600
#endif

#ifndef SERIAL_DELAY_MS
#define SERIAL_DELAY_MS 2000
#endif

#ifndef SERIAL_MESSAGE_DELIMITER
#define SERIAL_MESSAGE_DELIMITER ':'
#endif

enum class SerialMessageType
{
  SerialSync,
  Log,
};
const char *const SERIAL_MESSAGE_TYPE_STRINGS[] = {
    "SERIAL_SYNC",
    "LOG",
};

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
