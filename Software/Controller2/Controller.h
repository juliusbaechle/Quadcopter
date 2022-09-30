#pragma once

#include "ControllerState.h"

class Controller {
public:
  void begin();
  ControllerState getState();

private:
  void calibrate();
  bool getShutdown();

private:
  bool m_shutdown = true;
  ControllerState m_offset;
};