#pragma once

#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN 10
#define TIMEOUT_MS 500

class Receiver {
public:
  Receiver() : m_radio{ CE_PIN, CSN_PIN } {
    m_radio.begin();
    m_radio.setDataRate(RF24_250KBPS);
    m_radio.setRetries(3, 5);
    m_radio.openReadingPipe(0, slaveAddress);
    m_radio.setPALevel(RF24_PA_MAX);
    m_radio.startListening();
  }

  void setSerialOutputFlag(bool output) { m_output = output; }
  bool isConnected() { return m_connected; }
  bool isPowerOff() { return m_powerOff; }

  // Vertikale Beschleunigung in m/s^2: -10 m/s^2 ... +10 m/s^2
  float getAcc()   { return (float)  m_zAcc / 1000; }

  // Neigung in Rad: -45° ... +45°
  float getPitch() { return (float) m_pitch / 1000; }

  // Rollwinkel in Rad: -45° ... +45°
  float getRoll()  { return (float)  m_roll / 1000; }

  // Rotationsdrehrate in Rad: -1 U/s ... +1 U/s
  float getYaw()   { return (float)   (-m_yaw) / 1000; }

  void update() {
    if (millis() - m_lastPingMillis > TIMEOUT_MS) 
      m_connected = false;

    if (m_radio.available()) {
      char text[12] = "";
      m_radio.read(&text, sizeof(text));
      parse(text);
      if (m_output) printState();

      m_connected = true;
      m_lastPingMillis = millis();
    }
  }

private:
  void parse(char* text) {
    if (text[0] == 'O') {
      m_powerOff = true;
      m_zAcc = 0;
      m_pitch = 0;
      m_roll = 0;
      m_yaw = 0;
    }
    else {
      m_powerOff = false;

      for (int i = 0; i < 4; i++) {
        char id = text[3 * i];

        int16_t val = (int16_t)text[3 * i + 1] << 8;
        val |= text[3 * i + 2] & 0xFF;

        assign(id, val);
      }
    }
  }

  void assign(char id, int16_t val) {
    switch (id) {
      case 'A':
        m_zAcc = val;
        break;
      case 'R':
        m_roll = val;
        break;
      case 'P':
        m_pitch = val;        
        break;
      case 'Y':
        m_yaw = val;
        break;
    }
  }

  void printState() {
     Serial.print("A: ");   Serial.print(getAcc());
     Serial.print(", R: "); Serial.print(getRoll());
     Serial.print(", P: "); Serial.print(getPitch());
     Serial.print(", Y: "); Serial.println(getYaw());
  }

private:
  bool m_output = false;
  bool m_powerOff = true;
  bool m_connected = false;
  uint32_t m_lastPingMillis = 0;

  const byte slaveAddress[5] = { 'D', 'r', 'o', 'n', 'e' };   //RxAAA
  RF24 m_radio;

  int16_t m_zAcc = 0;
  int16_t m_roll = 0;
  int16_t m_pitch = 0;
  int16_t m_yaw = 0;
};