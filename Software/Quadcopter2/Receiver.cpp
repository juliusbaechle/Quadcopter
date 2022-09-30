#include "Receiver.h"

#define TIMEOUT_MS 500

void Receiver::begin() {
  m_rf24.begin(9, 10);
  m_rf24.setDataRate(RF24_250KBPS);
  m_rf24.setRetries(3, 5);
  m_rf24.openReadingPipe(0, m_address);
  m_rf24.setPALevel(RF24_PA_MAX);
  m_rf24.startListening();
}

void Receiver::update() {
  if (millis() - m_lastPingMs > TIMEOUT_MS)
    m_available = false;

  if (m_rf24.available()) {
    char text[15] = "";
    m_rf24.read(&text, sizeof(text));
    parse(text);
    m_lastPingMs = millis();
  }
}

// Message Format: S__C__P__R__Y__
void Receiver::parse(char* text) {
  for (int i = 0; i < 5; i++) {
    char id = text[3 * i];
    int16_t val = (int16_t)text[3 * i + 1] << 8;
    val |= text[3 * i + 2] & 0xFF;
    assign(id, val);
  }
}

void Receiver::assign(char id, int16_t val) {
  switch (id) {
  case 'S':
    m_available = val == 0;
    break;
  case 'C':
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