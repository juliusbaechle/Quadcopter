#pragma once

struct ProcessVars {
  ProcessVars() = default;
  ProcessVars operator- (const ProcessVars & a_vars);
  ProcessVars& operator+= (const ProcessVars& a_vars);
  ProcessVars& operator/= (float a_factor);

  float thrust = 0; // percent
  float roll = 0;  // rad
  float pitch = 0; // rad
  float yawrate = 0;   // rad/s
};

void print(const ProcessVars& a_vars);
void plot(const ProcessVars& a_vars);
