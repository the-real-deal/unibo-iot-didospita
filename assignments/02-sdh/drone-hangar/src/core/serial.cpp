#include "serial.hpp"

#include <Arduino.h>

#include "std/enum.hpp"

SerialMessageService::SerialMessageService(uint32_t baud, char messageDelimiter, char syncByte)
    : baud(baud), messageDelimiter(messageDelimiter), syncByte(syncByte),
      currentMessage(nullptr) {}

void SerialMessageService::setup()
{
  Serial.begin(this->baud);
  while (!Serial)
    ;
  
  Serial.print(this->syncByte);
  Serial.flush();
}

Message *SerialMessageService::getMessage() { return this->currentMessage; }

bool SerialMessageService::messageAvailable()
{
  return this->currentMessage != nullptr;
}

Message *SerialMessageService::readNewMessage()
{
  if (!Serial.available()) {
    return nullptr;
  }
  
  auto buffer = Serial.readString();

  int messageStartIndex = buffer.indexOf(this->messageDelimiter);
  if (messageStartIndex == -1)
  {
    return nullptr;
  }

  int typeDelimiterIndex = buffer.indexOf(this->messageDelimiter, messageStartIndex + 1);
  if (typeDelimiterIndex == -1)
  {
    return nullptr;
  }

  int terminatorIndex = buffer.indexOf(this->messageDelimiter, typeDelimiterIndex + 1);
  if (terminatorIndex == -1)
  {
    return nullptr;
  }

  String typeString = buffer.substring(messageStartIndex + 1, typeDelimiterIndex);
  String content = buffer.substring(typeDelimiterIndex + 1, terminatorIndex);

  MessageType type = enumFromString<MessageType>(typeString.c_str(), MESSAGE_TYPE_STRINGS);
  return new Message(type, content);
}

void SerialMessageService::read()
{
  delete this->currentMessage;
  auto newMessage = this->readNewMessage();
  this->currentMessage = newMessage;
}

void SerialMessageService::send(Message message)
{
  auto typeStr = enumToString<MessageType>(message.getType(), MESSAGE_TYPE_STRINGS);
  auto contentStr = message.getContent();

  Serial.flush();
  Serial.print(this->messageDelimiter);
  Serial.print(typeStr);
  Serial.print(this->messageDelimiter);
  Serial.print(contentStr);
  Serial.print(this->messageDelimiter);
  Serial.print('\n');
  Serial.flush();
}
