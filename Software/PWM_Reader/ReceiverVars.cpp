#include "ReceiverVars.h"
#include <Arduino.h>

ReceiverVars::ReceiverVars(bool a_on, short a_thrust, short a_roll, short a_pitch, short a_yawrate) {
  on = a_on;
  thrust = a_thrust;
  roll = a_roll;
  pitch = a_pitch;
  yawrate = a_yawrate;
}

ReceiverVars ReceiverVars::operator- (const ReceiverVars& a_vars) {
  ReceiverVars vars;
  vars.on = on;
  vars.thrust = thrust - a_vars.thrust;
  vars.roll = roll - a_vars.roll;
  vars.pitch = pitch - a_vars.pitch;
  vars.yawrate = yawrate - a_vars.yawrate;
  return vars;
}

void print(const ReceiverVars& a_vars) {
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
