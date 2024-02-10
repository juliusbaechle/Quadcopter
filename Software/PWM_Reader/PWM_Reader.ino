#include <Wire.h>
#include "ReceiverVars.h"

ReceiverVars offset = ReceiverVars(false, 14, 39, -133, 121);

ReceiverVars readReceiverVars() {
  ReceiverVars vars;
  vars.roll = map(pulseIn(2, HIGH), 900, 1950, 524, -524); // +- 30°
  vars.pitch = map(pulseIn(3, HIGH), 770, 1330, -524, 524); // +- 30°
  vars.thrust = map(pulseIn(4, HIGH), 1100, 2070, 0, 1000); // 0 - 100 %
  vars.yawrate = map(pulseIn(5, HIGH), 1030, 2050, -1571, 1571); // 90° / s

  auto val = pulseIn(6, HIGH);
  vars.on = (val > 100) && (val < 1450);
  return vars;
}

void send(const ReceiverVars& a_vars) {
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
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
}

void loop() {
  ReceiverVars vars = readReceiverVars() - offset;  // ~75 ms
  send(vars);
  print(vars);
}
