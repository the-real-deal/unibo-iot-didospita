#pragma once

#include <Arduino.h>
#include <LinkedList.h>

#include "io/message.hpp"
#include "scheduler.hpp"

class SerialMessageService : public MessageService, public ExternalInput {
 private:
  static const char MESSAGE_DELIMITER = '|';
  String serialBuffer;
  LinkedList<Message*> queue;
  Message* currentMessage;

 public:
  SerialMessageService();
  bool requireInterrupts() override;
  Message* getMessage() override;
  bool messageAvailable() override;
  void send(Message message) override;
  void read() override;
};
