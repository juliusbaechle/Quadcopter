#pragma once

#include <Servo.h>
#include "Thrust.h"

class Servos {
public:
  void begin();
  void set(Thrust a_thrust);

private:
  uint16_t toUs(float a_percent);

private:
  Servo m_servos[4];
};