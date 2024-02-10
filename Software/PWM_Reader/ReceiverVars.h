#pragma once

struct ReceiverVars {
  ReceiverVars() = default;
  ReceiverVars(bool a_on, short a_thrust, short a_roll, short a_pitch, short a_yawrate);
  ReceiverVars operator- (const ReceiverVars & a_vars);

  bool on = false;
  short thrust = 0; // promille
  short roll = 0;  // rad
  short pitch = 0; // rad
  short yawrate = 0;   // rad/s
};

void print(const ReceiverVars& a_vars);
