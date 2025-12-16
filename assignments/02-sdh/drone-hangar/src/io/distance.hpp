#pragma once

class DistanceSensor {
 public:
  virtual float getDistance() = 0;
  virtual ~DistanceSensor() = default;
};
