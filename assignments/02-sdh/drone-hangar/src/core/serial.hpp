#pragma once

#include <Arduino.h>
#include <LinkedList.h>

#include "io/message.hpp"
#include "scheduler.hpp"

class SerialMessageService : public MessageService, public ExternalInput {
 private:
  static const char MESSAGE_DELIMITER = '|';
  LinkedList<Message*> queue;
  Message* currentMessage;
  static Message* decodeSerialMessage(String message);

 public:
  SerialMessageService();
  Message* getMessage() override;
  bool messageAvailable() override;
  void send(Message message) override;
  void read() override;
};
