#include "Transmitter.h"

void Transmitter::begin() {
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setRetries(3, 5);
  radio.openWritingPipe(m_address);
  radio.setPALevel(RF24_PA_MAX);
}

void Transmitter::send(const ControllerState& a_state) {
  m_buffer[1] = 0;
  m_buffer[2] = a_state.shutdown;
  m_buffer[4] = highByte(a_state.climb);
  m_buffer[5] = lowByte(a_state.climb);
  m_buffer[7] = highByte(a_state.pitch);
  m_buffer[8] = lowByte(a_state.pitch);
  m_buffer[10] = highByte(a_state.roll);
  m_buffer[11] = lowByte(a_state.roll);
  m_buffer[13] = highByte(a_state.yaw);
  m_buffer[14] = lowByte(a_state.yaw);
  bool ack = radio.write(m_buffer, 15);
}