#include "dht.hpp"

DHTSensor::DHTSensor(uint8_t pin,
                     DHTType type,
                     float initialTemp,
                     float initialHumidity) : dht(pin, static_cast<uint8_t>(type)),
                                              temperature(initialTemp),
                                              humidity(initialHumidity),
                                              readTimer(0) {}

void DHTSensor::setup()
{
  this->dht.begin();
  sensor_t tempSensor, humiditySensor;
  this->dht.temperature().getSensor(&tempSensor);
  this->dht.humidity().getSensor(&humiditySensor);
  uint64_t readPeriodMs = max(max(0, tempSensor.min_delay), humiditySensor.min_delay) / 1000;
  this->readTimer = Timer(readPeriodMs);
}

// https://registry.platformio.org/libraries/adafruit/DHT%20sensor%20library/examples/DHT_Unified_Sensor/DHT_Unified_Sensor.ino
void DHTSensor::read()
{
  // if (!this->readTimer.isFinished())
  // {
  //   return;
  // }

  // Serial.println(F("TEMP READ START"));
  // Serial.flush();

  // sensors_event_t event;

  // dht.temperature().getEvent(&event);
  // if (!isnan(event.temperature))
  // {
  //   Serial.println(F("TEMP READ"));
  //   Serial.flush();
  //   this->temperature = event.temperature;
  // }

  // dht.humidity().getEvent(&event);
  // if (!isnan(event.relative_humidity))
  // {
  //   Serial.println(F("HUM READ"));
  //   Serial.flush();
  //   this->humidity = event.relative_humidity;
  // }

  // this->readTimer.reset();

  // Serial.println(F("TEMP READ END"));
  // Serial.flush();
}

float DHTSensor::getTemperature() { return this->temperature; }

float DHTSensor::getHumidity() { return this->humidity; }
