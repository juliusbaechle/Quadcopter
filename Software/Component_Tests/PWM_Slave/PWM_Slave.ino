#include <Wire.h>
#include "ProcessVars.h"

ProcessVars vars;

void setup() {
  Wire.begin(1);
  Wire.setClock(400000);
  Wire.onReceive(receive);
  Serial.begin(115200);
}

void receive() {
  static int pos = 0;
  char buf [13];
  
  while (Wire.available()) {
    char c = Wire.read();
    buf[pos++] = c;
    if (c == '-') {
      if (pos == 13)
        print(parse(buf));
      pos = 0;
    }
  }
}

// Message Format: T__P__R__Y__
ProcessVars parse(char* text) {
  ProcessVars vars;
  for (int i = 0; i < 4; i++) {
    char id = text[3 * i];
    int16_t val = (int16_t) ((text[3 * i + 1] << 8) | (text[3 * i + 2] & 0xFF));
    assign(vars, id, val);
  }
  return vars;
}

void assign(ProcessVars& vars, char id, int16_t val) {
  switch (id) {
  case 'T':
    vars.thrust = (float)(val) / 1000.0f;
    break;
  case 'P':
    vars.pitch = (float)(val) / 1000.0f;
    break;
  case 'R':
    vars.roll = (float)(val) / 1000.0f;
    break;
  case 'Y':
    vars.yawrate = (float)(val) / 1000.0f;
    break;
  }
}

void loop() {
}
