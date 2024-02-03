#include "ProcessVars.h"
#include <Arduino.h>

ProcessVars::ProcessVars(bool a_on, short a_thrust, short a_roll, short a_pitch, short a_yawrate) {
  on = a_on;
  thrust = a_thrust;
  roll = a_roll;
  pitch = a_pitch;
  yawrate = a_yawrate;
}

ProcessVars ProcessVars::operator- (const ProcessVars& a_vars) {
  ProcessVars vars;
  vars.on = on;
  vars.thrust = thrust - a_vars.thrust;
  vars.roll = roll - a_vars.roll;
  vars.pitch = pitch - a_vars.pitch;
  vars.yawrate = yawrate - a_vars.yawrate;
  return vars;
}

void print(const ProcessVars& a_vars) {
  Serial.print(F("On: "));
  Serial.print(a_vars.on);
  Serial.print(F(", Pitch: "));
  Serial.print(a_vars.pitch);
  Serial.print(F(", Roll: "));
  Serial.print(a_vars.roll);
  Serial.print(F(", Yaw: "));
  Serial.print(a_vars.yawrate);
  Serial.print(F(", Thrust: "));
  Serial.println(a_vars.thrust);
}
