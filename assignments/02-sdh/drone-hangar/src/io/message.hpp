#pragma once

#include <Arduino.h>

enum class MessageType {
  REQUEST_LANDING,
  REQUEST_TAKEOFF,
  STATE,
  DISTANCE,
};
const char* const MESSAGE_TYPE_STRINGS[] = {
    "REQUEST LANDING",
    "REQUEST TAKEOFF",
    "STATE",
    "DISTANCE",
};

class Message {
 private:
  MessageType type;
  String content;

 public:
  Message(MessageType type);
  Message(MessageType type, String content);
  MessageType getType();
  String getContent();
};

class MessageService {
 public:
  virtual Message* getMessage() = 0;
  virtual bool messageAvailable() = 0;
  virtual void send(Message message) = 0;
  virtual ~MessageService() = default;
};
