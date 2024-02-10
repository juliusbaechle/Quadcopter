#include "Receiver.h"
#include "ControlElements.h"
#include <Wire.h>

#define TIMEOUT_MS 500
#define NUM_VARS 5
#define BUF_SIZE (NUM_VARS * 3 + 1)

ProcessVars vars;
uint32_t lastPingMs = 0;
int pos = 0;
char buf [BUF_SIZE];
bool on = false;

const float T1_REC = 0.025;
PT2Element pt2_thrust = PT2Element(1, T1_REC, T1_REC);
PT2Element pt2_roll = PT2Element(1, T1_REC, T1_REC);
PT2Element pt2_pitch = PT2Element(1, T1_REC, T1_REC);
PT2Element pt2_yawrate = PT2Element(1, T1_REC, T1_REC);

void Receiver::begin() {
  Wire.begin(1);
  Wire.setClock(400000);
  Wire.onReceive(&Receiver::receive);
  Wire.setWireTimeout(10000, false);
}

ProcessVars Receiver::read(float interval_s, bool& ok) {
  ProcessVars smoothed_vars;
  ok &= on && !Wire.getWireTimeoutFlag();
  smoothed_vars.thrust = pt2_thrust(vars.thrust, interval_s);
  smoothed_vars.pitch = pt2_pitch(vars.pitch, interval_s);
  smoothed_vars.roll = pt2_roll(vars.roll, interval_s);
  smoothed_vars.yawrate = pt2_yawrate(vars.yawrate, interval_s);
  Wire.clearWireTimeoutFlag();
  return smoothed_vars; 
}

void Receiver::receive() {  
  while (Wire.available()) {
    lastPingMs = millis();
    char c = Wire.read();
    buf[pos++] = c;
    if (c == '-') {
      if (pos == BUF_SIZE)
        vars = parse(buf);
      pos = 0;
    }
  }
}

// Message Format: S__T__R__P__Y__
ProcessVars Receiver::parse(char* text) {
  ProcessVars vars;
  for (int i = 0; i < NUM_VARS; i++) {
    char id = text[3 * i];
    int16_t val = (int16_t) ((text[3 * i + 1] << 8) | (text[3 * i + 2] & 0xFF));
    assign(vars, id, val);
  }
  return vars;
}

void Receiver::assign(ProcessVars& vars, char id, int16_t val) {
  switch (id) {
  case 'S':
    on = bool(val);
    break;
  case 'T':
    vars.thrust = (float)(val) / 10.0f;
    break;
  case 'R':
    vars.roll = (float)(val) / 1000.0f;
    break;
  case 'P':
    vars.pitch = (float)(val) / 1000.0f;
    break;
  case 'Y':
    vars.yawrate = (float)(val) / 1000.0f;
    break;
  }
}
