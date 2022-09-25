#pragma once

#include <RF24.h>
#include "ProcessVars.h"

class Receiver {
public:
  void begin();
  void update();
  bool isAvailable() const { return m_available; }
  ProcessVars getVars() const { return m_vars; }

private:
  void parse(char* text);
  void assign(char id, int16_t val);

private:
  bool m_available = false;
  ProcessVars m_vars;
  uint32_t m_lastPingMs = 0;
  RF24 m_rf24;
};