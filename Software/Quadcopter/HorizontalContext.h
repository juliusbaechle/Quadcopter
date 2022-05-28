#pragma once

#include "StandardControllers.h"

class HorizontalContext {
public:
  virtual float getAngle() = 0;
  virtual float getRate() = 0;
  virtual float getAcc() = 0;
  virtual ts_servoValues toServoValues(float force) = 0;
};


class RollContext : public HorizontalContext {
public:
  RollContext(ISensor* sensor) { m_sensor = sensor; }

  float getRate() { return m_sensor->getRateY(); }
  float getAngle() { return m_sensor->getAngleY(); }
  float getAcc() { return m_sensor->getAccX(); }

  ts_servoValues toServoValues(float force) {
    ts_servoValues f;
    f.frontRight = -force;
    f.backRight  = -force;
    f.backLeft   =  force;
    f.frontLeft  =  force;
    return f;
  }

private:
  ISensor* m_sensor;
};


class PitchContext : public HorizontalContext {
public:
  PitchContext(ISensor* sensor) { m_sensor = sensor; }

  float getRate() { return m_sensor->getRateX(); }
  float getAngle() { return m_sensor->getAngleX(); }
  float getAcc() { return m_sensor->getAccY(); }

  ts_servoValues toServoValues(float force) {
    ts_servoValues servoValues;
    servoValues.frontRight = -force;
    servoValues.backRight  =  force;
    servoValues.backLeft   =  force;
    servoValues.frontLeft  = -force;
    return servoValues;
  }

private:
  ISensor* m_sensor;
};