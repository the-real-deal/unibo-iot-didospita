#include "serial.hpp"

#include "std/enum.hpp"

SerialMessageService::SerialMessageService()
    : serialBuffer(), queue(), n_messages(0), currentMessage(nullptr) {}

Message *SerialMessageService::getMessage() { return this->currentMessage; }

bool SerialMessageService::messageAvailable()
{
  return this->currentMessage != nullptr;
}

void SerialMessageService::read()
{
  if (Serial.available())
  {
    auto buffer = Serial.readStringUntil('\n');

    if (this->serialBuffer.length() == 0)
    {
      auto delimiterIndex = buffer.indexOf(MESSAGE_DELIMITER);
      if (delimiterIndex != -1)
      {
        this->serialBuffer = buffer.substring(delimiterIndex);
      }
    }
    else
    {
      this->serialBuffer += buffer;
    }
  }

  // Try to parse complete messages from buffer
  // Format: |TYPE|CONTENT|
  while (this->serialBuffer.length() > 0)
  {
    int messageStartIndex = this->serialBuffer.indexOf(MESSAGE_DELIMITER);
    if (messageStartIndex == -1)
    {
      this->serialBuffer = String(); // remove extra garbage that is not part of a message
      break;
    }

    int typeDelimiterIndex = this->serialBuffer.indexOf(MESSAGE_DELIMITER, messageStartIndex + 1);
    if (typeDelimiterIndex == -1)
    {
      break;
    }

    int terminatorIndex = this->serialBuffer.indexOf(MESSAGE_DELIMITER, typeDelimiterIndex + 1);
    if (terminatorIndex == -1)
    {
      break;
    }

    // We have a complete message!
    String typeStr = this->serialBuffer.substring(messageStartIndex + 1, typeDelimiterIndex);
    String content = this->serialBuffer.substring(typeDelimiterIndex + 1, terminatorIndex);

    // Parse message type
    MessageType type = enumFromString<MessageType>(typeStr, MESSAGE_TYPE_STRINGS);
    auto message = new Message(type, content);
    if (this->n_messages < SERIAL_MESSAGES_QUEUE_SIZE) {
      this->queue[this->n_messages] = message;
      this->n_messages++;
    }
    this->serialBuffer = this->serialBuffer.substring(terminatorIndex + 1);
  }

  // Get next message from queue
  if (this->messageAvailable())
  {
    delete this->currentMessage;
  }
  if (this->n_messages > 0) {
    this->currentMessage = this->queue[this->n_messages - 1];
    this->n_messages--;  
  } else {
    this->currentMessage = nullptr;
  }
}

void SerialMessageService::send(Message message)
{
  String typeString = String(
      enumToString<MessageType>(message.getType(), MESSAGE_TYPE_STRINGS));
  Serial.flush();
  Serial.print(MESSAGE_DELIMITER);
  Serial.print(typeString);
  Serial.print(MESSAGE_DELIMITER);
  Serial.print(message.getContent());
  Serial.print(MESSAGE_DELIMITER);
  Serial.print('\n');
  Serial.flush();
}
