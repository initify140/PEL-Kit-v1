#include "pel_sensors.h"

static const uint8_t analogPins[] = {
  A0, // PEL_LDR
  A1, // PEL_NTC
  A2  // PEL_MQ
};

int pel_readAnalog(PEL_AnalogSensor s) {
  return analogRead(analogPins[s]);
}
