#include "serial.hpp"

#include <Arduino.h>

#include "std/enum.hpp"

SerialManager::SerialManager(EventFamily family)
    : SyncEventSource(family) {}

void SerialManager::begin(EventsManager *eventsManager)
{
  EventSource<SerialMessage>::begin(eventsManager);
  Serial.begin(SERIAL_BAUD);
  delay(SERIAL_DELAY_MS);
  this->serialSync();
}

void SerialManager::checkEvents()
{
  if (!Serial.available())
  {
    return;
  }

  String buffer = Serial.readStringUntil('\n');
  if (buffer.endsWith("\r"))
  {
    buffer.remove(buffer.length() - 1);
  }

  int delimiterIndex = buffer.indexOf(SERIAL_MESSAGE_DELIMITER);
  if (delimiterIndex == -1)
  {
    return;
  }

  String typeString = buffer.substring(0, delimiterIndex);
  String content = buffer.substring(delimiterIndex + 1, buffer.length());

  SerialMessageType type = enumFromString<SerialMessageType>(typeString.c_str(), SERIAL_MESSAGE_TYPE_STRINGS);
  SerialMessage message = {.type = type};
  size_t copyLength = min(sizeof(message.data) / sizeof(char) - 1, content.length());
  strncpy(message.data, content.c_str(), copyLength);
  message.data[copyLength] = '\0';
  this->generateEvent(message);
}

void SerialManager::sendMessage(SerialMessageType type, const char *message)
{
  const char *typeString = enumToString(type, SERIAL_MESSAGE_TYPE_STRINGS);
  if (message == nullptr)
  {
    Serial.println(typeString);
  }
  else
  {
    Serial.print(typeString);
    Serial.print(SERIAL_MESSAGE_DELIMITER);
    Serial.println(message);
  }
  Serial.flush();
}

void SerialManager::serialSync()
{
  this->sendMessage(SerialMessageType::SerialSync, nullptr);
}

void SerialManager::log(const char *message)
{
  this->sendMessage(SerialMessageType::Log, message);
}

void SerialManager::sendStatus(SystemStatus status)
{
  const char *message = enumToString<SystemStatus>(status, SYSTEM_STATUS_STRINGS);
  this->sendMessage(SerialMessageType::Status, message);
}

void SerialManager::sendAngle(int angle)
{
  String message = String(angle);
  this->sendMessage(SerialMessageType::Angle, message.c_str());
}
