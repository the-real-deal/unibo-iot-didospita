#include "serial.hpp"

#include <assert.h>

#include "utils.hpp"

SerialMessageService::SerialMessageService()
    : queue(), currentMessage(nullptr) {}

Message* SerialMessageService::getMessage() { return this->currentMessage; }

bool SerialMessageService::messageAvailable() {
  return this->currentMessage != nullptr;
}

Message* SerialMessageService::decodeSerialMessage(String message) {
  assert(message.length() >= 3);            // start, type delimiter, terminator
  assert(message[0] == MESSAGE_DELIMITER);  // start with delimiter
  int typeDelimiterIndex = message.indexOf(MESSAGE_DELIMITER);
  assert(typeDelimiterIndex != -1);
  int terminatorIndex =
      message.indexOf(MESSAGE_DELIMITER, typeDelimiterIndex + 1);
  assert(terminatorIndex == ((int)message.length()) - 1);
  MessageType type = enumFromString<MessageType>(
      message.substring(0, typeDelimiterIndex), MESSAGE_TYPE_STRINGS);
  String content = message.substring(typeDelimiterIndex, terminatorIndex);
  return new Message(type, content);
}

void SerialMessageService::read() {
  String content;
  uint8_t delimiters = 0;

  while (Serial.available()) {
    char ch = (char)Serial.read();
    if (ch == MESSAGE_DELIMITER) {
      delimiters++;
    }
    if (delimiters > 0) {
      content += ch;
    }
    if (delimiters == 3) {
      this->queue.add(SerialMessageService::decodeSerialMessage(content));
      content = String();
      delimiters = 0;
    }
  }
  delete this->currentMessage;
  this->currentMessage = this->queue.size() > 0 ? this->queue.pop() : nullptr;
}

void SerialMessageService::send(Message message) {
  String typeString = String(
      enumToString<MessageType>(message.getType(), MESSAGE_TYPE_STRINGS));
  Serial.flush();
  Serial.print(MESSAGE_DELIMITER);
  Serial.print(typeString);
  Serial.print(MESSAGE_DELIMITER);
  Serial.print(message.getContent());
  Serial.print(MESSAGE_DELIMITER);
  Serial.flush();
}
