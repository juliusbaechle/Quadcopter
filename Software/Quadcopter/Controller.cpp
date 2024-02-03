#include "Controller.h"
#include <Arduino.h>

Controller::Controller() 
  : m_pitchController(10, 1, 3, 0.01)
  , m_rollController(10, 1, 3, 0.01)
  , m_yawController(5, 0, 1, 0.01)
{}

Thrust Controller::calc(ProcessVars a_diff, float intervalS) {
  Thrust thrust (a_diff.thrust);
  thrust += calcPitch(a_diff.pitch, intervalS);
  thrust += calcRoll(a_diff.roll, intervalS);
  thrust += calcYaw(a_diff.yawrate, intervalS);
  return thrust;
}

Thrust Controller::calcPitch(float a_diff, float a_intervalS) {
  float val = m_pitchController(a_diff, a_intervalS);
  return Thrust(-val, -val, val, val);
}

Thrust Controller::calcRoll(float a_diff, float a_intervalS) {
  float val = m_rollController(a_diff, a_intervalS);
  return Thrust(val, -val, val, -val);
}

Thrust Controller::calcYaw(float a_diff, float a_intervalS) {
  float val = m_yawController(a_diff, a_intervalS);
  return Thrust(val, -val, -val, val);
}

void Controller::reset() {
  m_pitchController.reset();
  m_yawController.reset();
  m_rollController.reset();
}
