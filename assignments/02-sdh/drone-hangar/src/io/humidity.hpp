#pragma once

class HumiditySensor {
 public:
  virtual float getHumidity() = 0;
  virtual ~HumiditySensor() = default;
};
