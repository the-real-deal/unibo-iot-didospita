#pragma once

#include <DHT.h>

#include "core/scheduler.hpp"
#include "io/temperature.hpp"

enum class DHTType : uint8_t
{
  DHT11 = DHT11,
  DHT21 = DHT21,
  DHT22 = DHT22,
};

class DHTSensor : public TemperatureSensor,
                  public ExternalInput
{
private:
  DHT dht;
  float temperature;

protected:
  void setup() override;
  void read() override;

public:
  DHTSensor(uint8_t pin, DHTType type, float initialTemp);
  float getTemperature() override;
};
