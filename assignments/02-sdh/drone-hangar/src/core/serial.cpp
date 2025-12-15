#include "serial.hpp"
#include "utils.hpp"
#include <assert.h>

SerialManager::SerialManager(unsigned long baud) : queue() {
  Serial.begin(baud);
  while (!Serial) {
  }
}

Message *SerialManager::getMessage() { return this->currentMessage; }

bool SerialManager::messageAvailable() {
  return this->currentMessage != nullptr;
}

Message *SerialManager::decodeSerialMessage(String message) {
  assert(message.length() >= 3);           // start, type delimiter, terminator
  assert(message[0] == MESSAGE_DELIMITER); // start with delimiter
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

void SerialManager::read() {
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
      this->queue.add(SerialManager::decodeSerialMessage(content));
      content = String();
      delimiters = 0;
    }
  }
  this->currentMessage = this->queue.size() > 0 ? this->queue.pop() : nullptr;
}

void SerialManager::send(Message *message) {
  Serial.print(
      MESSAGE_DELIMITER +
      enumToString<MessageType>(message->getType(), MESSAGE_TYPE_STRINGS) +
      MESSAGE_DELIMITER + message->getContent() + MESSAGE_DELIMITER);
}
