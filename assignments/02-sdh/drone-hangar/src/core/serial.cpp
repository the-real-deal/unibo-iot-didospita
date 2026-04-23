#include "serial.hpp"

#include <Arduino.h>
#include <MemoryUsage.h>

#include "std/enum.hpp"

SerialMessageService::SerialMessageService(uint64_t baud, char messageDelimiter, char syncByte)
    : baud(baud), messageDelimiter(messageDelimiter), syncByte(syncByte),
      queue(), n_messages(0), currentMessage(nullptr) {}

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

void SerialMessageService::readMessages()
{
  String buffer;
  while (Serial.available())
  {
    String buffer = Serial.readString();

    int messageStartIndex = buffer.indexOf(this->messageDelimiter);
    if (messageStartIndex == -1)
    {
      return;
    }

    int typeDelimiterIndex = buffer.indexOf(this->messageDelimiter, messageStartIndex + 1);
    if (typeDelimiterIndex == -1)
    {
      return;
    }

    int terminatorIndex = buffer.indexOf(this->messageDelimiter, typeDelimiterIndex + 1);
    if (terminatorIndex == -1)
    {
      return;
    }

    // We have a complete message!
    String typeStr = buffer.substring(messageStartIndex + 1, typeDelimiterIndex);
    String content = buffer.substring(typeDelimiterIndex + 1, terminatorIndex);

    // Parse message type
    MessageType type = enumFromString<MessageType>(typeStr.c_str(), MESSAGE_TYPE_STRINGS);
    auto message = new Message(type, content);
    if (this->n_messages < SERIAL_MESSAGES_QUEUE_SIZE)
    {
      this->queue[this->n_messages] = message;
      this->n_messages++;
    }
    buffer = buffer.substring(terminatorIndex);
  }
}

void SerialMessageService::read()
{
  this->readMessages();

  if (this->messageAvailable())
  {
    delete this->currentMessage;
  }
  if (this->n_messages > 0)
  {
    this->currentMessage = this->queue[this->n_messages - 1];
    this->n_messages--;
  }
  else
  {
    this->currentMessage = nullptr;
  }
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
