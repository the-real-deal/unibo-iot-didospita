#pragma once

class PresenceSensor {
public:
  virtual bool isPresent() = 0;
  virtual ~PresenceSensor() = default;
};
