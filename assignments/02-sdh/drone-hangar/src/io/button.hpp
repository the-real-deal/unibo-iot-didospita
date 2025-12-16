#pragma once

class Button {
public:
  virtual bool isPressed() = 0;
  virtual ~Button() = default;
};
