#pragma once
#include <Arduino.h>
#include "pel_pins.h"

namespace pel {

void servoInit();
void servoWrite(PEL_Servo s, uint8_t angle);

}
