#include "sonar.hpp"

UltrasonicSensor::UltrasonicSensor(uint8_t echoPin, uint8_t triggerPin,
                                   uint64_t readDelayMicros,
                                   TemperatureSensor *temperatureSensor)
    : echoPin(echoPin), triggerPin(triggerPin),
      readDelayMicros(readDelayMicros), temperatureSensor(temperatureSensor),
      staticTemperature(NAN), distance(NAN) {}

UltrasonicSensor::UltrasonicSensor(uint8_t echoPin, uint8_t triggerPin,
                                   uint64_t readDelayMicros, float temperature)
    : echoPin(echoPin), triggerPin(triggerPin),
      readDelayMicros(readDelayMicros), temperatureSensor(nullptr),
      staticTemperature(temperature), distance(NAN) {}

float UltrasonicSensor::getTemperature() {
  return this->temperatureSensor == nullptr
             ? this->staticTemperature
             : this->temperatureSensor->getTemperature();
}

float UltrasonicSensor::getSoundSpeed() {
  return 331.5 + 0.6 * this->getTemperature();
}

// https://github.com/pslab-unibo/esiot-2025-2026/blob/18463be0de31a90caf9740de2df1c6108cb350b3/lab-activities/lab-activity-08/sweeping-system/arduino/sweeping-system/src/devices/Sonar.cpp#L29-L30
float UltrasonicSensor::pulseToDistance(float pulse) {
  return pulse / 1000.0 / 1000.0 / 2 * this->getSoundSpeed();
}

void UltrasonicSensor::read() {
  this->triggerPin.write(DigitalValue::Low);
  delayMicroseconds(this->readDelayMicros);
  this->triggerPin.write(DigitalValue::High);
  delayMicroseconds(this->readDelayMicros);
  this->triggerPin.write(DigitalValue::Low);

  float pulse = this->echoPin.readPulse(DigitalValue::High);
  this->distance = pulse == 0 ? NAN : this->pulseToDistance(pulse);
}

float UltrasonicSensor::getDistance() { return this->distance; }

bool UltrasonicSensor::isOutOfRange() { return this->distance == NAN; }
