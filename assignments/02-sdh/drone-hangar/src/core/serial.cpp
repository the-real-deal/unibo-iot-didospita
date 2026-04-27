#include "serial.hpp"

#include <Arduino.h>

#include "std/enum.hpp"

SerialMessageService::SerialMessageService(uint64_t baud, char messageDelimiter, char syncByte)
    : baud(baud), messageDelimiter(messageDelimiter), syncByte(syncByte),
      currentMessage(nullptr) {}

void SerialMessageService::begin()
{
  Serial.begin(this->baud);
  while (!Serial)
    ;
}

Message *SerialMessageService::getMessage() { return this->currentMessage; }

bool SerialMessageService::messageAvailable()
{
  return this->currentMessage != nullptr;
}

void SerialMessageService::setup()
{
  Serial.print(this->syncByte);
  Serial.flush();
}

Message* SerialMessageService::readNewMessage()
{
  while (Serial.available())
  {
    this->buffer += Serial.readString();
  }

  int messageStartIndex = this->buffer.indexOf(this->messageDelimiter);
  if (messageStartIndex == -1)
  {
    return nullptr;
  }

  int typeDelimiterIndex = this->buffer.indexOf(this->messageDelimiter, messageStartIndex + 1);
  if (typeDelimiterIndex == -1)
  {
    return nullptr;
  }

  int terminatorIndex = this->buffer.indexOf(this->messageDelimiter, typeDelimiterIndex + 1);
  if (terminatorIndex == -1)
  {
    return nullptr;
  }

  String typeStr = this->buffer.substring(messageStartIndex + 1, typeDelimiterIndex);
  String content = this->buffer.substring(typeDelimiterIndex + 1, terminatorIndex);

  MessageType type = enumFromString<MessageType>(typeStr.c_str(), MESSAGE_TYPE_STRINGS);
  auto message = new Message(type, content);
  buffer = buffer.substring(terminatorIndex);
  return message;
}

void SerialMessageService::read()
{
  auto newMessage = this->readNewMessage();

  if (this->messageAvailable())
  {
    delete this->currentMessage;
  }

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
