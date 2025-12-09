#include "serial.hpp"
#include "utils.hpp"
#include <assert.h>

Message::Message(String message) {
  size_t delimiterIndex = message.indexOf(Message::DELIMITER);
  size_t terminatorIndex = message.indexOf(Message::TERMINATOR);
  this->type = enumFromString<MessageType>(message.substring(0,delimiterIndex), MESSAGE_TYPE_STRINGS);
  this->content = message.substring(delimiterIndex, terminatorIndex);
}

Message::Message(MessageType type, String content) : type(type), content(content) {}

MessageType Message::getType() {
    return this->type;
}

String Message::getContent() {
    return this->content;
}

String Message::toString() {
    return enumToString<MessageType>(this->type, MESSAGE_TYPE_STRINGS) + Message::DELIMITER + this->content + Message::TERMINATOR;
}

Message SerialManager::getMessage() {
    assert(this->messageAvailable());
    // no pop first method available
    Message message = this->queue.get(0);
    this->queue.remove(0);
    return message;
}

bool SerialManager::messageAvailable() {
    return this->queue.size() != 0;
}

void SerialManager::read() {
/* reading the content */
  String content;
  while (Serial.available()) {
    char ch = (char) Serial.read();
    if (ch == Message::TERMINATOR){
      this->queue.add(Message(content + Message::TERMINATOR));
      content = String();   
    } else {
      content += ch;      
    }
  }
}

void SerialManager::send(Message* message) {
  Serial.print(message->toString());
}