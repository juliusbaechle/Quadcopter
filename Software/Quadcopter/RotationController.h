#pragma once

class RotationController { // Nicht getestet !
public:
  RotationController(IGyrosensor* gyro) {
    m_gyro = gyro;
  }

  ts_servoValues calculate(float rotationSetpoint, float interval_s) {
    float servoValues = m_pid(rotationSetpoint - m_gyro->getRateZ(), interval_s);
    return toServoValues(servoValues);
  }

  ts_servoValues toServoValues(float servoValues) {
    ts_servoValues f;
    f.frontRight =  servoValues;
    f.backRight  = -servoValues;
    f.backLeft   =  servoValues;
    f.frontLeft  = -servoValues;
    return f;
  }

  void reset() {
    m_pid.reset();
  }

private:
  IGyrosensor* m_gyro;
  PIDController m_pid { ROT_KP, ROT_KI, ROT_KD };
};