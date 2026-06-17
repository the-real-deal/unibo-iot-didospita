#pragma once

#include <Arduino.h>

#include "kernel/events.hpp"
#include "core/system.hpp"

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
#define SERIAL_MESSAGE_DATA_BUF_LEN 20
#endif

enum class SerialMessageType
{
  Log,
  State,
  Door,
};
const char *const SERIAL_MESSAGE_TYPE_STRINGS[] = {
    "LOG",
    "STATE",
    "DOOR",
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

protected:
  void generateEvents() override;

public:
  SerialManager(EventFamily family);
  void begin(EventsManager *eventsManager) override;
  void log(const char *message);
  void sendState(SystemState state);
  void sendDoorOpening(float percentage);
};
