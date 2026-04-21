#include "dht.hpp"

DHTSensor::DHTSensor(DHT dht) : dht(dht) {}

void DHTSensor::setup()
{
  this->dht.begin();
}

// https://registry.platformio.org/libraries/adafruit/DHT%20sensor%20library/examples/DHT_Unified_Sensor/DHT_Unified_Sensor.ino
void DHTSensor::read()
{
  this->temperature = this->dht.readTemperature();
  this->humidity = this->dht.readHumidity();
}

float DHTSensor::getTemperature() { return this->temperature; }

float DHTSensor::getHumidity() { return this->humidity; }
