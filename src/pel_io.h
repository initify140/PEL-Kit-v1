#pragma once
#include <Arduino.h>

namespace pel {

class Led {
public:
  explicit Led(uint8_t pin);
  void on();
  void off();

private:
  uint8_t _pin;
};

Led led(uint8_t pin);

}