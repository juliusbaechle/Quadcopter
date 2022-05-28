#pragma once

#include <Servo.h>
#include <Arduino.h>
#include "ts_servoValues.h"

#define NEWTON2US 217
#define US2NEWTON 0.0046

class IActuator {
public:
  // Force (single servo) [N]
  virtual void setAll(float force) = 0;
  // Force [FrontRight, BackRight, BackLeft, FrontLeft] [N]
  virtual void set(ts_servoValues servoValues) = 0;
  
  // Sum of Force [N]
  float getTotalForce() {
    float totalForce = 0;
    totalForce += m_servoValues.frontRight;
    totalForce += m_servoValues.backRight;
    totalForce += m_servoValues.backLeft;
    totalForce += m_servoValues.frontLeft;
    return totalForce;
  }

protected:
  ts_servoValues m_servoValues;
};

class SerialActuator : public IActuator {
public:
  SerialActuator(bool serialOut = true) {
    m_serialOut = serialOut;
  }

  void setAll(float force) {
    m_servoValues = force;

    if (!m_serialOut) return;
    Serial.print(F("Set All: "));
    Serial.print(force * US2NEWTON);
    Serial.println(F(" N"));
  }

  void set(ts_servoValues servoValues) {
    m_servoValues = servoValues;

    if (!m_serialOut) return;
    Serial.print(F("Set: FR "));
    Serial.print(servoValues.frontRight * US2NEWTON); Serial.print(F(" N, BR "));
    Serial.print(servoValues.backRight * US2NEWTON); Serial.print(F(" N, BL "));
    Serial.print(servoValues.backLeft * US2NEWTON); Serial.print(F(" N, FL "));
    Serial.print(servoValues.frontLeft * US2NEWTON); Serial.println(F(" N"));
  }

private:
  bool m_serialOut;
};


class Servos : public IActuator {
public:
  Servos() {
    m_servos[0].attach(8); //FR / ESC2
    m_servos[1].attach(5); //BR / ESC1
    m_servos[2].attach(7); //BL / ESC3
    m_servos[3].attach(4); //FL / ESC4
  }

  void setAll(float value) {
    m_servoValues = value;
    for (int i = 0; i < 4; i++) {
      m_servos[i].writeMicroseconds(toUs(value));
    }
  }

  void set(ts_servoValues servoValues) {
    m_servoValues = servoValues;
    m_servos[0].writeMicroseconds(toUs(servoValues.frontRight));
    m_servos[1].writeMicroseconds(toUs(servoValues.backRight ));
    m_servos[2].writeMicroseconds(toUs(servoValues.backLeft  ));
    m_servos[3].writeMicroseconds(toUs(servoValues.frontLeft ));
  }

private:
  uint16_t toUs(float promille) {
    if (promille > 1000) promille = 1000;
    if (promille < 0) promille = 0;
    return 1000 + (uint16_t)promille;
  }

private:
  Servo m_servos[4];
};
