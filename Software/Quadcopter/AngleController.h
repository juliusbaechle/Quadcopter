#pragma once

#include "StandardControllers.h"
#include "HorizontalContext.h"
#include "RateController.h"
#include "Params.h"

class AngleController { // Getestet !
public:
  AngleController(HorizontalContext* context, RateController* tiltController)
    : m_tiltController(tiltController)
    , m_context(context)
  {
  }

  ts_servoValues calculate(float angleSetpoint, float interval_s) {
    angleSetpoint = LIMIT(angleSetpoint, ANGLE_LIMIT);
    float angleDiff = angleSetpoint - m_context->getAngle();
    float omegaSetpoint = m_pid(angleDiff, interval_s);
    return m_tiltController->calculate(omegaSetpoint, interval_s);
  }

  void reset() {
    m_pid.reset();
    m_tiltController->reset();
  }

private:
  HorizontalContext* m_context;
  RateController* m_tiltController;
  PIDController m_pid{ ANGLE_KP, ANGLE_KI, ANGLE_KD };
};