#pragma once

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include "core/scheduler.hpp"
#include "io/humidity.hpp"
#include "io/temperature.hpp"

enum class DHTType : uint8_t
{
  DHT11 = DHT11,
  DHT21 = DHT21,
  DHT22 = DHT22,
};

class DHTSensor : public TemperatureSensor,
                  public HumiditySensor,
                  public ExternalInput
{
private:
  DHT dht;
  float temperature;
  float humidity;

protected:
  void setup() override;
  void read() override;

public:
  DHTSensor(DHT dht);
  float getTemperature() override;
  float getHumidity() override;
};
