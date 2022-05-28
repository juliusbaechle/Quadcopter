#pragma once

#include "StandardControllers.h"
#include "HorizontalContext.h"
#include "Params.h"

class RateController { // Getestet !
public:
  RateController(HorizontalContext* context) {
    m_context = context;
  }

  ts_servoValues calculate(float omegaSetpoint, float interval_s) {
    float omegaDiff = omegaSetpoint - m_context->getRate();
    float force = m_pid(omegaDiff, interval_s);
    return m_context->toServoValues(force);
  }

  void reset() {
    m_pid.reset();
  }

private:
  HorizontalContext* m_context;
  PIDController m_pid{ RATE_KP, RATE_KI, RATE_KD };
};