#include "TestSensor.hpp"

TestSensor::TestSensor(uint8_t pin) : pin(pin, AnalogInputPin::NO_SCALE) {}

void TestSensor::read() {
  this->value = this->pin.read();
  Serial.println("TestSensor: " + String(this->value));
}
