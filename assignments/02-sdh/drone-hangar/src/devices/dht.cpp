#include "dht.hpp"
#include <assert.h>

DHTSensor::DHTSensor(uint8_t pin, DHTType type)
    : dht(pin, static_cast<uint8_t>(type)) {
  this->dht.begin();
}

// https://registry.platformio.org/libraries/adafruit/DHT%20sensor%20library/examples/DHT_Unified_Sensor/DHT_Unified_Sensor.ino

void DHTSensor::read() {
  this->dht.temperature().getEvent(&this->event);
  this->dht.humidity().getEvent(&this->event);
  assert(!isnan(this->event.temperature));
  assert(!isnan(this->event.relative_humidity));
}

float DHTSensor::getTemperature() { return this->event.temperature; }

float DHTSensor::getHumidity() { return this->event.relative_humidity; }
