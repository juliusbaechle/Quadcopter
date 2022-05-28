#pragma once

#include "Actuator.h"
#include "Receiver.h"
#include "Sensor.h"

#include "AngleController.h"
#include "AccelerationController.h"
#include "RotationController.h"

enum EState {
  PowerOff,
  Flying,
  Landing
};

class Controlling {
public:
  Controlling(Receiver* receiver, IActuator* actuator, ISensor* sensor);
  void update(float interval_s);

private:
  void PowerOff();
  void Flying(float interval_s);
  void Landing(float interval_s);
  bool isLanded(float interval_s);
  EState transitNewState(float interval_s);

private:
  Receiver* m_receiver;
  IActuator* m_actuator;
  ISensor* m_sensor;

  AccelerationController* m_accCntrl;
  RotationController* m_rotCntrl;

  HorizontalContext* m_rollContext;
  RateController* m_pitchRateCntrl;
  AngleController* m_pitchAngleCntrl; 

  HorizontalContext* m_pitchContext;
  RateController* m_rollRateCntrl;
  AngleController* m_rollAngleCntrl;

private:
  EState m_state = EState::PowerOff;
  float m_timeAccZero_s = 0;
  bool m_powerOff = true; // Merker für PowerOn Flanke
};