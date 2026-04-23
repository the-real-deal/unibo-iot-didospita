#pragma once

#include <Arduino.h>

#include "config.h"
#include "io/message.hpp"
#include "scheduler.hpp"
#include "begin.hpp"

class SerialMessageService : public MessageService, public ExternalInput, public DelayedBegin
{
private:
  uint64_t baud;
  char messageDelimiter;
  char syncByte;
  Message *queue[SERIAL_MESSAGES_QUEUE_SIZE];
  size_t n_messages;
  Message *currentMessage;

private:
  void readMessages();

protected:
  void setup() override;
  void read() override;

public:
  void begin() override;
  SerialMessageService(uint64_t baud, char messageDelimiter, char syncByte);
  Message *getMessage() override;
  bool messageAvailable() override;
  void send(Message message) override;
};
