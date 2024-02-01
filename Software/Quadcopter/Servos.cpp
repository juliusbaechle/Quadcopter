#include "Servos.h"
#include <HardwareSerial.h>

void Servos::begin() {
  m_servos[0].attach(8); // FL / ESC4
  m_servos[1].attach(5); // FR / ESC2
  m_servos[2].attach(4); // RL / ESC3
  m_servos[3].attach(7); // RR / ESC1
  for (int i = 0; i < 4; i++)
    m_servos[i].writeMicroseconds(1000);
}

void Servos::set(Thrust a_thrust) { 
  m_servos[0].writeMicroseconds(toUs(a_thrust.frontLeft));
  m_servos[1].writeMicroseconds(toUs(a_thrust.frontRight));
  m_servos[2].writeMicroseconds(toUs(a_thrust.rearLeft));
  m_servos[3].writeMicroseconds(toUs(a_thrust.rearRight));
}

uint16_t Servos::toUs(float a_percent) {
  if (a_percent > 100)
    a_percent = 100;
  if (a_percent < 0) 
    a_percent = 0;
  return 1000 + (uint16_t)(10 * a_percent);
}