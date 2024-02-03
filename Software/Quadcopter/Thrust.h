#pragma once

struct Thrust {
  Thrust(float a_value = 0.0);
  Thrust(float a_fl, float a_fr, float a_rl, float a_rr);
  Thrust& operator+= (const Thrust& a_thrust);

  float frontLeft = 0.0;  // %
  float frontRight = 0.0; // %
  float rearLeft = 0.0;   // %
  float rearRight = 0.0;  // %
};

void print(const Thrust& a_thrust);
