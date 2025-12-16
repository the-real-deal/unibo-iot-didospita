#pragma once

class ServoMotor {
 public:
  virtual int getAngle() = 0;
  virtual void setAngle(int angle) = 0;
  virtual ~ServoMotor() = default;
};
