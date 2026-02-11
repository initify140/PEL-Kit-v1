#include <Arduino.h>

namespace pel {

void begin() {
  // status LED pinMode
  pinMode(PEL_LED_STATUS, OUTPUT);
}

}
