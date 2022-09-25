#include "Receiver.h"

#define TIMEOUT_MS 500

const byte slaveAddress[5] = { 'D', 'r', 'o', 'n', 'e' };

void Receiver::begin() {
  m_rf24.begin(9, 10);
  m_rf24.setDataRate(RF24_250KBPS);
  m_rf24.setRetries(3, 5);
  m_rf24.openReadingPipe(0, slaveAddress);
  m_rf24.setPALevel(RF24_PA_MAX);
  m_rf24.startListening();
}

void Receiver::update() {
  if (millis() - m_lastPingMs > TIMEOUT_MS)
    m_available = false;

  if (m_rf24.available()) {
    char text[12] = "";
    m_rf24.read(&text, sizeof(text));
    parse(text);
    m_lastPingMs = millis();
  }
}

void Receiver::parse(char* text) {
  if (text[0] == 'O') {
    m_rf24.flush_rx();
    m_available = false;
    m_vars = ProcessVars();
  } else {
    m_available = true;
    for (int i = 0; i < 4; i++) {
      char id = text[3 * i];
      int16_t val = (int16_t)text[3 * i + 1] << 8;
      val |= text[3 * i + 2] & 0xFF;
      assign(id, val);
    }
  }
}

void Receiver::assign(char id, int16_t val) {
  switch (id) {
  case 'A':
    m_vars.climb = (float)(val) / 1000.0f;
    break;
  case 'R':
    m_vars.roll = (float)(val) / 1000.0f;
    break;
  case 'P':
    m_vars.pitch = (float)(val) / 1000.0f;
    break;
  case 'Y':
    m_vars.yaw = (float)(val) / 1000.0f;
    break;
  }
}