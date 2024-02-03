#pragma once

#include "ProcessVars.h"
#include "Thrust.h"
#include "ControlElements.h"

class Controller {
public:
  Controller();
  Thrust calc(ProcessVars a_diff, float a_intervalS);
  void reset();

private:
  Thrust calcPitch(float a_diff, float a_intervalS);
  Thrust calcRoll(float a_diff, float a_intervalS);
  Thrust calcYaw(float a_diff, float a_intervalS);

private:
  PIDT2Element m_pitchController;
  PIDT2Element m_yawController;
  PIDT2Element m_rollController;
};
