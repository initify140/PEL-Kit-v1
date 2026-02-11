#include "pel_io.h"

namespace pel {

Led::Led(uint8_t pin) : _pin(pin) {
  pinMode(_pin, OUTPUT);
}

void Led::on() {
  digitalWrite(_pin, HIGH);
}

void Led::off() {
  digitalWrite(_pin, LOW);
}

Led led(uint8_t pin) {
  return Led(pin);
}

}
