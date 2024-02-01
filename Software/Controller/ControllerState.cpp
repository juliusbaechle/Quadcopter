#include "ControllerState.h"
#include <HardwareSerial.h>

void print(const ControllerState& a_state) {
  Serial.print("Shutdown: ");
  Serial.print(a_state.shutdown ? "true" : "false");
  Serial.print(", Climb: ");
  Serial.print(a_state.climb);
  Serial.print(", Pitch: ");
  Serial.print(a_state.pitch);
  Serial.print(", Roll: ");
  Serial.print(a_state.roll);
  Serial.print(", Yawrate: ");
  Serial.println(a_state.yaw);
}
