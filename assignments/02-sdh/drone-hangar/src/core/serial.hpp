#pragma once

#include "scheduler.hpp"
#include <Arduino.h>
#include <LinkedList.h>

enum class MessageType {
  STATE,
  DISTANCE,
};
const String MESSAGE_TYPE_STRINGS[] = {
    "STATE",
    "DISTANCE",
};

class Message {
private:
  String content;
  MessageType type;

public:
  static const char DELIMITER = '|';
  static const char TERMINATOR = '\n';

  Message(String message);
  Message(MessageType type, String content);
  MessageType getType();
  String getContent();
  String toString();
};

class SerialManager : public ExternalInput {
private:
  LinkedList<Message> queue;

public:
  Message getMessage();
  bool messageAvailable();
  void read() override;
  void send(Message *message);
};
