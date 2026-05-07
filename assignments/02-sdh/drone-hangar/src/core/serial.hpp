#pragma once

#include <Arduino.h>

#include "config.h"
#include "io/message.hpp"
#include "scheduler.hpp"
#include "setup.hpp"

class SerialMessageService : public MessageService, public ExternalInput
{
private:
  uint32_t baud;
  char messageDelimiter;
  char syncByte;
  Message *currentMessage;

private:
  Message *readNewMessage();

protected:
  void read() override;

public:
  SerialMessageService(uint32_t baud, char messageDelimiter, char syncByte);

  void setup() override;
  Message *getMessage() override;
  bool messageAvailable() override;
  void send(Message message) override;
};
