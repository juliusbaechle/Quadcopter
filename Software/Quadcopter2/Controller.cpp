#include "Controller.h"
#include <Arduino.h>

// Maximum Roll/Pitch P: 25
// Maximum 

Controller::Controller() 
  : m_climbController(10, 30, 0)
  , m_pitchController(13, 0, 50)
  , m_rollController(13, 0, 50)
  , m_yawController(3, 0, 0)
{}

Thrust Controller::calc(ProcessVars a_diff) {
  Thrust thrust (15);
  float intervalS = getIntervalS();
  // thrust += calcClimb(a_diff.climb, intervalS);
  thrust += calcPitch(a_diff.pitch, intervalS);
  thrust += calcRoll(a_diff.roll, intervalS);
  thrust += calcYaw(a_diff.yaw, intervalS);
  return thrust;
}

float Controller::getIntervalS() {
  unsigned long intervalMicros = 0;
  auto currentMicros = micros();
  intervalMicros = currentMicros - m_previousMicros;
  m_previousMicros = currentMicros;
  return (float)(intervalMicros) / 1000000;
}

Thrust Controller::calcClimb(float a_diff, float a_intervalS) {
  float val = m_climbController(a_diff, a_intervalS);
  return Thrust(val, val, val, val);
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
  m_climbController.reset();
  m_pitchController.reset();
  m_yawController.reset();
  m_rollController.reset();
  m_previousMicros = micros();
}