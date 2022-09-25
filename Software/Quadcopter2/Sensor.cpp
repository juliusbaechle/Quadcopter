#include "Sensor.h"

void Sensor::begin() {
  bool connected = false;
  for (int i = 0; i < 3; i++) {
    connected |= m_sensor.begin();
    delay(200);
  }

  if (connected) {
    m_sensor.setAccelerometerRange(MPU6050_RANGE_4_G); // +- 4 g        (2, 4, 8, 16)
    m_sensor.setGyroRange(MPU6050_RANGE_500_DEG);      // +- 500 °/s    (250, 500, 1000, 2000)
    m_sensor.setFilterBandwidth(MPU6050_BAND_94_HZ);   // 94 Hz         (260, 184, 94, 44, 21, 10, 5)
    calibrate();
  } else {
    Serial.println(F("Failed to find MPU6050 chip"));
  }
}

void Sensor::calibrate() {
  static const int sampleCount = 1000;
  Serial.println(F("Calculating Offsets!"));

  for (int i = 0; i < sampleCount; i++) {
    if (i % (sampleCount / 3) == 0)
      Serial.print(".");
    m_offsets += readSensor();
  }
  m_offsets /= sampleCount;

  Serial.println();
  Serial.print("Offsets: ");
  print(m_offsets);
}

ProcessVars Sensor::getVars() {
  return readSensor() - m_offsets;
}

ProcessVars Sensor::readSensor() {
  ProcessVars vars;
  sensors_event_t g;
  m_sensor.getGyroSensor()->getEvent(&g);
  vars.roll += g.gyro.x;
  vars.pitch += g.gyro.y;
  vars.yaw += g.gyro.z;

  sensors_event_t a;
  m_sensor.getAccelerometerSensor()->getEvent(&a);
  vars.climb += a.acceleration.z;
  return vars;
}