#pragma once

#include <RF24.h>
#include "ProcessVars.h"

class Receiver {
public:
  void begin();
  static ProcessVars read(float a_intervalS, bool& ok);
  static void receive();

private:
  static ProcessVars parse(char* text);
  static void assign(ProcessVars& vars, char id, int16_t val);
};
