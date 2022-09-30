#include "Controller.h"
#include <Arduino.h>

void Controller::begin() {
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  m_offset = getState();
  Serial.print("Offset: ");
  print(m_offset);
}

ControllerState Controller::getState() {
  ControllerState state;
  state.shutdown = getShutdown();
  state.climb = map(analogRead(A0), 0, 1024, -3000, 3000) - m_offset.climb;
  state.yaw = map(analogRead(A1), 0, 1024, -1570, 1570) - m_offset.yaw;
  state.pitch = map(analogRead(A2), 0, 1024, -785, 785) - m_offset.pitch;
  state.roll = map(analogRead(A3), 0, 1024, -785, 785) - m_offset.roll;
  return state;
}

bool Controller::getShutdown() {
  if (!digitalRead(2))
    m_shutdown = false;
  if (!digitalRead(3))
    m_shutdown = true;
  return m_shutdown;
}