#include <Wire.h>
#include "ProcessVars.h"

ProcessVars offset = ProcessVars(false, 14, 39, -133, 121);

ProcessVars desiredVars() {
  ProcessVars vars;
  vars.roll = map(pulseIn(2, HIGH), 900, 1950, -785, 785);
  vars.pitch = map(pulseIn(3, HIGH), 770, 1330, -785, 785);
  vars.thrust = map(pulseIn(4, HIGH), 1100, 2070, 0, 1000);
  vars.yawrate = map(pulseIn(5, HIGH), 1030, 2050, -3141, 3141);

  auto val = pulseIn(6, HIGH);
  vars.on = (val > 100) && (val < 1450);
  return vars;
}

void send(const ProcessVars& a_vars) {
  char m_buffer[16] = "S__T__P__R__Y__-";
  m_buffer[1] = 0;
  m_buffer[2] = a_vars.on;
  m_buffer[4] = highByte(a_vars.thrust);
  m_buffer[5] = lowByte(a_vars.thrust);
  m_buffer[7] = highByte(a_vars.pitch);
  m_buffer[8] = lowByte(a_vars.pitch);
  m_buffer[10] = highByte(a_vars.roll);
  m_buffer[11] = lowByte(a_vars.roll);
  m_buffer[13] = highByte(a_vars.yawrate);
  m_buffer[14] = lowByte(a_vars.yawrate);

  Wire.beginTransmission(1);
  Wire.write(m_buffer, 16);
  Wire.endTransmission();
}

void setup() {
  Wire.begin();  
  Wire.setClock(400000);
  Serial.begin(115200);
}

void loop() {
  ProcessVars vars = desiredVars() - offset;  // ~75 ms
  send(vars);
  print(vars);
}
