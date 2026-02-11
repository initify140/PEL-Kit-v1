#include "pel_servos.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

static Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver();

void pel::servoInit() {
  Wire.begin();
  pca.begin();
  pca.setPWMFreq(50);
}

void pel::servoWrite(PEL_Servo s, uint8_t angle) {
  uint16_t pulse = map(angle, 0, 180, 150, 600);
  pca.setPWM((uint8_t)s, 0, pulse);
}
