#include "serial.hpp"

#include <Arduino.h>

#include "std/enum.hpp"

SerialManager::SerialManager() {}

void SerialManager::setup()
{
  Serial.begin(SERIAL_BAUD);
  delay(SERIAL_DELAY_MS);
  this->serialSync();
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
