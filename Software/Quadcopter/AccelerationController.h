#pragma once

#include "StandardControllers.h"
#include "ts_servoValues.h"
#include "Params.h"

class AccelerationController {
public:
  AccelerationController(ISensor* sensor) {
    m_sensor = sensor;
  }

  ts_servoValues calculate(float setpoint, float interval_s) {
    float actualAcc = m_sensor->getAccZ() - m_sensor->getGravity();
    float thrust = m_pi(setpoint - actualAcc, interval_s);
    thrust = compensateAngle(thrust);
    return toServoValues(thrust);
  }

  void reset() {
    m_pi.reset();
  }

private:
  float compensateAngle(float thrust) {
    float angleX = m_sensor->getAngleX();
    float angleY = m_sensor->getAngleY();
    float angleToZ = sqrt(angleX * angleX + angleY * angleY);
    return thrust / cos(angleToZ);
  }

  ts_servoValues toServoValues(float thrust) {
    ts_servoValues servoValues;
    servoValues += thrust / 4;
    return servoValues;
  }

private:
  ISensor* m_sensor;
  PIController m_pi { Z_ACC_KP, Z_ACC_KI };
};