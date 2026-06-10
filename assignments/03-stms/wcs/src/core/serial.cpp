#include "serial.hpp"

#include <Arduino.h>

#include "std/enum.hpp"

SerialManager::SerialManager() {}

void SerialManager::setup()
{
  Serial.begin(ARDUINO_SERIAL_BAUD);
  while (!Serial)
    ;
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
    Serial.print(":");
    Serial.println(message);
  }
  Serial.flush();
}

void SerialManager::serialSync()
{
  this->sendMessage(SerialMessageType::SERIAL_SYNC, nullptr);
}

void SerialManager::log(const char *message)
{
  this->sendMessage(SerialMessageType::LOG, message);
}
