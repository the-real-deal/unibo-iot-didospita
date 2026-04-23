#pragma once

#include <Arduino.h>

#include "config.h"
#include "io/message.hpp"
#include "scheduler.hpp"

class SerialMessageService : public MessageService, public ExternalInput
{
private:
  char messageDelimiter;
  char syncByte;
  String serialBuffer;
  Message *queue[SERIAL_MESSAGES_QUEUE_SIZE];
  size_t n_messages;
  Message *currentMessage;

protected:
  void setup() override;
  void read() override;

public:
  SerialMessageService(char messageDelimiter, char syncByte);
  Message *getMessage() override;
  bool messageAvailable() override;
  void send(Message message) override;
};
