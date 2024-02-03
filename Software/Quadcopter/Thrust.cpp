#include "Thrust.h"
#include <HardwareSerial.h>

Thrust::Thrust(float a_value) {
  frontLeft = a_value;
  frontRight = a_value;
  rearLeft = a_value;
  rearRight = a_value;
}

Thrust::Thrust(float a_fl, float a_fr, float a_rl, float a_rr) {
  frontLeft = a_fl;
  frontRight = a_fr;
  rearLeft = a_rl;
  rearRight = a_rr;
}

Thrust& Thrust::operator+=(const Thrust& a_thrust) {
  frontLeft += a_thrust.frontLeft;
  frontRight += a_thrust.frontRight;
  rearLeft += a_thrust.rearLeft;
  rearRight += a_thrust.rearRight;
  return *this;
}

void print(const Thrust& a_thrust) {
  Serial.print(F("FL: "));
  Serial.print(100 * a_thrust.frontLeft);
  Serial.print(F(", FR: "));
  Serial.print(100 * a_thrust.frontRight);
  Serial.print(F(", RL: "));
  Serial.print(100 * a_thrust.rearLeft);
  Serial.print(F(", RR: "));
  Serial.println(100 * a_thrust.rearRight);
}
