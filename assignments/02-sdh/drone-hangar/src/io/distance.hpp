#pragma once

class DistanceSensor
{
public:
  virtual float getDistanceMm() = 0;
  virtual ~DistanceSensor() = default;
};
