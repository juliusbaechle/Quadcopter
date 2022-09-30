#pragma once

#include <RF24.h>
#include "ControllerState.h"

class Transmitter {
public:
  void begin();
  void send(const ControllerState& a_state);

private:
  char m_buffer[16] = "S__C__P__R__Y__";
  const byte m_address[5] = { 'D', 'r', 'o', 'n', 'e' };
  RF24 radio {9, 10};
};