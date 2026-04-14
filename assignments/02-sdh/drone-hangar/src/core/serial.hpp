#pragma once

#include <Arduino.h>

#include "config.h"
#include "io/message.hpp"
#include "scheduler.hpp"

class SerialMessageService : public MessageService, public ExternalInput
{
private:
  static const char MESSAGE_DELIMITER = '|';
  String serialBuffer;
  Message *queue[SERIAL_MESSAGES_QUEUE_SIZE];
  size_t n_messages;
  Message *currentMessage;

public:
  SerialMessageService();
  Message *getMessage() override;
  bool messageAvailable() override;
  void send(Message message) override;
  void read() override;
};
