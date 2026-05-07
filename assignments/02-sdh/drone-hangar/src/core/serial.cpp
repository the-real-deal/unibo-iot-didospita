#include "serial.hpp"

#include <Arduino.h>

#include "config.h"
#include "std/enum.hpp"

SerialMessageService::SerialMessageService() : currentMessage(nullptr) {}

void SerialMessageService::setup()
{
  Serial.begin(SERIAL_BAUD);
  while (!Serial)
    ;

  Serial.print(SERIAL_SYNC_BYTE);
  Serial.flush();
}

Message *SerialMessageService::getMessage() { return this->currentMessage; }

bool SerialMessageService::messageAvailable()
{
  return this->currentMessage != nullptr;
}

Message *SerialMessageService::readNewMessage()
{
  if (!Serial.available())
  {
    return nullptr;
  }

  auto buffer = Serial.readString();

  int messageStartIndex = buffer.indexOf(SERIAL_MESSAGE_DELIMITER);
  if (messageStartIndex == -1)
  {
    return nullptr;
  }

  int typeDelimiterIndex = buffer.indexOf(SERIAL_MESSAGE_DELIMITER, messageStartIndex + 1);
  if (typeDelimiterIndex == -1)
  {
    return nullptr;
  }

  int terminatorIndex = buffer.indexOf(SERIAL_MESSAGE_DELIMITER, typeDelimiterIndex + 1);
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
  Serial.print(SERIAL_MESSAGE_DELIMITER);
  Serial.print(typeStr);
  Serial.print(SERIAL_MESSAGE_DELIMITER);
  Serial.print(contentStr);
  Serial.print(SERIAL_MESSAGE_DELIMITER);
  Serial.print('\n');
  Serial.flush();
}
