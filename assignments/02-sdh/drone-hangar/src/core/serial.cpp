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
  static String buffer = "";
  
  // Read ALL available bytes into buffer
  while (Serial.available()) {
    char ch = Serial.read();
    if (ch != '\r' && ch != '\n') {
      buffer += ch;
    }
  }
  
  // If buffer is empty, nothing to do
  if (buffer.length() == 0) return;
  
  // Try to parse complete messages from buffer
  // Format: |TYPE|CONTENT|
  while (buffer.length() >= 3) {
    int firstDelim = buffer.indexOf('|');
    if (firstDelim == -1) {
      buffer = "";
      break;
    }
    
    int secondDelim = buffer.indexOf('|', firstDelim + 1);
    if (secondDelim == -1) {
      // Incomplete message - wait for more data
      break;
    }
    
    int thirdDelim = buffer.indexOf('|', secondDelim + 1);
    if (thirdDelim == -1) {
      // Incomplete - wait for more
      break;
    }
    
    // We have a complete message!
    String typeStr = buffer.substring(firstDelim + 1, secondDelim);
    String content = buffer.substring(secondDelim + 1, thirdDelim);
    
    Serial.print("PARSED:");
    Serial.print(typeStr);
    Serial.print(":");
    Serial.println(content);
    
    // Parse message type
    MessageType type = enumFromString<MessageType>(typeStr, MESSAGE_TYPE_STRINGS);
    this->queue.add(new Message(type, content));
    
    // Remove parsed message from buffer
    buffer = buffer.substring(thirdDelim + 1);
  }
  
  // Clean up buffer if it gets too long (possible garbage)
  if (buffer.length() > 50) {
    buffer = "";
  }
  
  // Get next message from queue
  if (this->messageAvailable()) {
    delete this->currentMessage;
  }
  this->currentMessage = this->queue.size() > 0 ? this->queue.pop() : nullptr;
  
  if (this->currentMessage != nullptr) {
    Serial.print("GOT:");
    Serial.println((int)this->currentMessage->getType());
  }
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
