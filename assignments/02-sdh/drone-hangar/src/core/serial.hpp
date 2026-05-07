#pragma once

#include <Arduino.h>

#include "config.h"
#include "io/message.hpp"
#include "scheduler.hpp"
#include "setup.hpp"

class SerialMessageService : public MessageService, public ExternalInput
{
private:
  Message *currentMessage;

private:
  Message *readNewMessage();

protected:
  void read() override;

public:
  SerialMessageService();

  void setup() override;
  Message *getMessage() override;
  bool messageAvailable() override;
  void send(Message message) override;
};
