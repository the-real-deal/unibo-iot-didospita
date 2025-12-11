#pragma once

class DistanceSensor {
public:
  virtual float getDistance() = 0;
  virtual bool isOutOfRange() = 0;
  virtual ~DistanceSensor() = default;
};
