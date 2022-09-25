#pragma once

#include "ProcessVars.h"
#include "Thrust.h"
#include "ControlElements.h"

class Controller {
public:
  Controller();
  Thrust calc(ProcessVars a_diff);
  void reset();

private:
  float getIntervalS();
  Thrust calcClimb(float a_diff, float a_intervalS);
  Thrust calcPitch(float a_diff, float a_intervalS);
  Thrust calcRoll(float a_diff, float a_intervalS);
  Thrust calcYaw(float a_diff, float a_intervalS);

private:
  unsigned long m_previousMicros = 0;
  PIDT1Element m_climbController;
  PIDT1Element m_pitchController;
  PIDT1Element m_yawController;
  PIDT1Element m_rollController;
};