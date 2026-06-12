#pragma once

#include <Arduino.h>

#include "core/events.hpp"

#ifndef SERIAL_BAUD
#define SERIAL_BAUD 9600
#endif

#ifndef SERIAL_DELAY_MS
#define SERIAL_DELAY_MS 2000
#endif

#ifndef SERIAL_MESSAGE_DELIMITER
#define SERIAL_MESSAGE_DELIMITER ':'
#endif

#ifndef SERIAL_MESSAGE_DATA_BUF_LEN
#define SERIAL_MESSAGE_DATA_BUF_LEN 10
#endif

enum class SerialMessageType
{
  SerialSync,
  Log,
  Mode,
};
const char *const SERIAL_MESSAGE_TYPE_STRINGS[] = {
    "SERIAL_SYNC",
    "LOG",
    "MODE",
};

struct SerialMessage
{
  SerialMessageType type;
  char data[SERIAL_MESSAGE_DATA_BUF_LEN];
};

class SerialManager : public SyncEventSource<SerialMessage>
{
private:
  void sendMessage(SerialMessageType type, const char *message);

public:
  SerialManager(EventFamily family);
  void begin(EventsManager *eventsManager) override;
  void checkEvents() override;
  void serialSync();
  void log(const char *message);
};
