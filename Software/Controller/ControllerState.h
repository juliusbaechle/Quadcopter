#pragma once

struct ControllerState {
  bool shutdown = false;
  int climb = 0; // mm/s^2
  int roll = 0;  // mrad/s
  int pitch = 0; // mrad/s
  int yaw = 0;   // mrad/s
};

void print(const ControllerState& a_state);