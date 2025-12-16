#pragma once

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include "core/scheduler.hpp"
#include "io/humidity.hpp"
#include "io/temperature.hpp"

enum class DHTType : uint8_t {
  DHT11 = DHT11,
  DHT21 = DHT21,
  DHT22 = DHT22,
};

class DHTSensor : public TemperatureSensor,
                  public HumiditySensor,
                  public ExternalInput {
 private:
  DHT_Unified dht;
  sensors_event_t event;

 protected:
  void read() override;

 public:
  DHTSensor(uint8_t pin, DHTType type);
  float getTemperature() override;
  float getHumidity() override;
};
