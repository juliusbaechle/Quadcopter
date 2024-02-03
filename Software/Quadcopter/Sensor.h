#pragma once

#include <Adafruit_MPU6050.h>
#include "ProcessVars.h"
#include "ControlElements.h"

const float T1_SENSOR = 0.01;

struct Coordinates {
  float x = 0;
  float y = 0;
  float z = 0;

  Coordinates() { x = 0; y = 0; z = 0; };
  Coordinates(const Coordinates& c) { x = c.x; y = c.y; z = c.z; };
  Coordinates(float a_x, float a_y, float a_z){ x = a_x; y = a_y; z = a_z; }
  
  Coordinates operator*(float f) { return Coordinates(f * x, f * y, f * z); }
  Coordinates operator/(float f) { return Coordinates(x / f, y / f, z / f); }
  Coordinates operator+(const Coordinates& c) { return Coordinates(x + c.x, y + c.y, z + c.z); }
  Coordinates operator-(const Coordinates& c) { return Coordinates(x - c.x, y - c.y, z - c.z); }

  const Coordinates& operator*=(float f) { x *= f; y *= f; z *= f; return *this; }
  const Coordinates& operator/=(float f) { x /= f; y /= f; z /= f; return *this; }
  const Coordinates& operator+=(const Coordinates& c) { x += c.x; y += c.y; z += c.z; return *this; }
  const Coordinates& operator-=(const Coordinates& c) { x -= c.x; y -= c.y; z -= c.z; return *this; }
};


static void println(const Coordinates& c) {
  Serial.print(c.x);
  Serial.print(", ");
  Serial.print(c.y);
  Serial.print(", ");
  Serial.println(c.z);
}


class Sensor {
public:
  void begin();
  ProcessVars read(float interval_s);

private:
  Coordinates readRates();
  Coordinates readAccAngles(); 

private:
  Adafruit_MPU6050 m_sensor;
  PT2Element pt2_roll = PT2Element(1, T1_SENSOR, T1_SENSOR);
  PT2Element pt2_pitch = PT2Element(1, T1_SENSOR, T1_SENSOR);
  PT2Element pt2_yawrate = PT2Element(1, T1_SENSOR, T1_SENSOR);
  Coordinates m_offset_rates = { -0.01, -0.06, -0.01 };
  Coordinates m_offset_angles = { -0.07, -0.02, 0 };
  Coordinates m_angles;
  uint64_t m_us;
};
