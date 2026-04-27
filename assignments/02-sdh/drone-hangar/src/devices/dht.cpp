#include "dht.hpp"

DHTSensor::DHTSensor(uint8_t pin,
                     DHTType type,
                     float initialTemp) : dht(pin, static_cast<uint8_t>(type)),
                                              temperature(initialTemp) {}

void DHTSensor::setup()
{
  this->dht.begin();
}

void DHTSensor::read()
{
  // auto temp = this->dht.readTemperature();
  auto temp = 28.40;
  if (!isnan(temp))
  {
    this->temperature = temp;
  }
}

float DHTSensor::getTemperature() { return this->temperature; }
