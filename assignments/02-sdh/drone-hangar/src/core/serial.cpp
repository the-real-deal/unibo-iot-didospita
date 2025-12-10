#include "serial.hpp"
#include "utils.hpp"
#include <assert.h>

Message::Message(String message) {
  assert(message.length() >= 3);            // start, type delimiter, terminator
  assert(message[0] == Message::DELIMITER); // start with delimiter
  int typeDelimiterIndex = message.indexOf(Message::DELIMITER);
  assert(typeDelimiterIndex != -1);
  int terminatorIndex =
      message.indexOf(Message::DELIMITER, typeDelimiterIndex + 1);
  assert(terminatorIndex == ((int)message.length()) - 1);
  this->type = enumFromString<MessageType>(
      message.substring(0, typeDelimiterIndex), MESSAGE_TYPE_STRINGS);
  this->content = message.substring(typeDelimiterIndex, terminatorIndex);
}

Message::Message(MessageType type, String content)
    : type(type), content(content) {}

MessageType Message::getType() { return this->type; }

String Message::getContent() { return this->content; }

String Message::toString() {
  return Message::DELIMITER +
         enumToString<MessageType>(this->type, MESSAGE_TYPE_STRINGS) +
         Message::DELIMITER + this->content + Message::DELIMITER;
}

SerialManager::SerialManager(unsigned long baud) : queue() {
  Serial.begin(baud);
  while (!Serial) {
  }
}

Message *SerialManager::getMessage(MessageType type) {
  if (!this->messageAvailable()) {
    return nullptr;
  }
  int index = -1;
  for (int i = 0; i < this->queue.size(); i++) {
    Message *message = this->queue.get(i);
    if (message->getType() == type) {
      index = i;
      break;
    }
  }
  if (index == -1) {
    return nullptr;
  }
  Message *message = this->queue.get(index);
  this->queue.remove(index);
  return message;
}

bool SerialManager::messageAvailable() { return this->queue.size() != 0; }

void SerialManager::read() {
  String content;
  uint8_t delimiters = 0;

  while (Serial.available()) {
    char ch = (char)Serial.read();
    if (ch == Message::DELIMITER) {
      delimiters++;
    }
    if (delimiters > 0) {
      content += ch;
    }
    if (delimiters == 3) {
      this->queue.add(new Message(content));
      content = String();
      delimiters = 0;
    }
  }
}

void SerialManager::send(Message *message) {
  Serial.print(message->toString());
}
