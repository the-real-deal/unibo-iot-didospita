#include "sonar.hpp"

UltrasonicSensor::UltrasonicSensor(uint8_t echoPin, uint8_t triggerPin,
                                   uint64_t readTimeoutMicros,
                                   uint64_t readDelayMicros,
                                   TemperatureSensor* tempSensor)
    : echoPin(echoPin),
      triggerPin(triggerPin),
      readTimeoutMicros(readTimeoutMicros),
      readDelayMicros(readDelayMicros),
      tempSensor(tempSensor),
      staticTemperature(NAN),
      distance(NAN) {}

UltrasonicSensor::UltrasonicSensor(uint8_t echoPin, uint8_t triggerPin,
                                   uint64_t readTimeoutMicros,
                                   uint64_t readDelayMicros, float temperature)
    : echoPin(echoPin),
      triggerPin(triggerPin),
      readTimeoutMicros(readTimeoutMicros),
      readDelayMicros(readDelayMicros),
      tempSensor(nullptr),
      staticTemperature(temperature),
      distance(NAN) {}

float UltrasonicSensor::getTemperature() {
  return this->tempSensor == nullptr ? this->staticTemperature
                                     : this->tempSensor->getTemperature();
}

float UltrasonicSensor::getSoundSpeed() {
  return 331.5 + 0.6 * this->getTemperature();
}

// https://github.com/pslab-unibo/esiot-2025-2026/blob/18463be0de31a90caf9740de2df1c6108cb350b3/lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src/devices/Sonar.cpp#L29-L30
float UltrasonicSensor::pulseToDistance(uint64_t pulse) {
  return ((float)pulse) / 1000.0 / 1000.0 / 2 * this->getSoundSpeed();
}

void UltrasonicSensor::read() {
  this->triggerPin.write(DigitalValue::Low);
  delayMicroseconds(this->readDelayMicros);
  this->triggerPin.write(DigitalValue::High);
  delayMicroseconds(this->readDelayMicros);
  this->triggerPin.write(DigitalValue::Low);

  uint8_t readStart = micros();
  uint64_t pulse =
      this->echoPin.readPulse(DigitalValue::High, this->readTimeoutMicros);
  uint64_t readTime = micros() - readStart;
  if (pulse == 0) {
    this->distance = readTime < this->readTimeoutMicros
                         ? UltrasonicSensor::DISTANCE_OOO_MIN
                         : UltrasonicSensor::DISTANCE_OOO_MAX;
  } else {
    this->distance = this->pulseToDistance(pulse);
  }
}

float UltrasonicSensor::getDistance() { return this->distance; }
