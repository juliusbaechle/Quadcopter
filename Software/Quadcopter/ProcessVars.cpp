#include "ProcessVars.h"
#include <Arduino.h>

ProcessVars ProcessVars::operator- (const ProcessVars& a_vars) {
  ProcessVars vars;
  vars.climb = climb - a_vars.climb;
  vars.roll = roll - a_vars.roll;
  vars.pitch = pitch - a_vars.pitch;
  vars.yawrate = yawrate - a_vars.yawrate;
  return vars;
}

ProcessVars& ProcessVars::operator+= (const ProcessVars& a_vars) {
  climb += a_vars.climb;
  roll += a_vars.roll;
  pitch += a_vars.pitch;
  yawrate += a_vars.yawrate;
  return *this;
}

ProcessVars& ProcessVars::operator/= (float a_factor) {
  climb /= a_factor;
  roll /= a_factor;
  pitch /= a_factor;
  yawrate /= a_factor;
  return *this;
}

void print(const ProcessVars& a_vars) {
  Serial.print(F("Pitch: "));
  Serial.print(a_vars.pitch);
  Serial.print(F(", Roll: "));
  Serial.print(a_vars.roll);
  Serial.print(F(", Yaw: "));
  Serial.print(a_vars.yawrate);
  Serial.print(F(", climb: "));
  Serial.println(a_vars.climb);
}
