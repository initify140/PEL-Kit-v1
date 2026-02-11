#pragma once
#include <Arduino.h>

namespace pel {

enum class Output {
  LED_STATUS,
  BUZZER
};

enum class Input {
  BTN_USER
};

enum class AnalogSensor {
  LDR,
  NTC,
  MQ
};

enum class Servo {
  SERVO_1,
  SERVO_2,
  SERVO_3,
  SERVO_4,
  SERVO_5,
  SERVO_6
};

namespace pins {
  static const uint8_t LED_STATUS = 13;
}

}
