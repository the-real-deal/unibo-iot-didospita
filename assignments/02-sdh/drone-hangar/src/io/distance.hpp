#pragma once

class DistanceSensor {
public:
  virtual float getDistance() = 0;
  virtual bool isTooFar() = 0;
  virtual bool isTooNear() = 0;
  virtual bool isOutOfRange() { return this->isTooFar() || this->isTooNear(); };
  virtual ~DistanceSensor() = default;
};
