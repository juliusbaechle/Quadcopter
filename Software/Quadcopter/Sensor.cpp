#include "Sensor.h"

void Sensor::begin() {
  bool connected = false;
  for (int i = 0; i < 3; i++) {
    connected |= m_sensor.begin();
    delay(200);
  }

  if (connected) {
    m_sensor.setAccelerometerRange(MPU6050_RANGE_4_G); // +- 4 g        (2, 4, 8, 16)
    m_sensor.setGyroRange(MPU6050_RANGE_500_DEG);      // +- 500 �/s    (250, 500, 1000, 2000)
    m_sensor.setFilterBandwidth(MPU6050_BAND_94_HZ);   // 94 Hz         (260, 184, 94, 44, 21, 10, 5)
    calibrate();
  } else {
    Serial.println(F("Failed to find MPU6050 chip"));
  }
}

void Sensor::calibrate() {
  static const int sampleCount = 1000;
    
  Serial.print(F("Calculating Offsets Gyro : "));
  for (int i = 0; i < sampleCount; i++) {
    if (i % (sampleCount / 3) == 0)
      Serial.print(".");
    m_offset_rates += readRates();
  }
  m_offset_rates /= sampleCount;
  Serial.println();

  Serial.print(F("Calculating Offsets Angle: "));
  for (int i = 0; i < sampleCount; i++) {
    if (i % (sampleCount / 3) == 0)
      Serial.print(".");
    m_offset_angles += readAccAngles();
  }
  m_offset_angles /= sampleCount;
  Serial.println();

  Serial.print("Offsets Rates : ");
  println(m_offset_rates);
  Serial.print("Offsets Angles: ");
  println(m_offset_angles);
}

ProcessVars Sensor::read(float interval_s) {
  const float K = 0.002; // influence acc : gyro
    
  Coordinates rates = readRates() - m_offset_rates;
  m_angles.x += interval_s * rates.x;
  m_angles.y += interval_s * rates.y;
  
  Coordinates acc_angles = readAccAngles() - m_offset_angles;
  m_angles.x += K * (acc_angles.x - m_angles.x);
  m_angles.y += K * (acc_angles.y - m_angles.y);

  sensors_event_t a;
  m_sensor.getAccelerometerSensor()->getEvent(&a);

  ProcessVars vars;
  vars.yawrate = rates.z;
  vars.pitch = m_angles.x;
  vars.roll = m_angles.y;
  vars.climb += interval_s * a.acceleration.z;
  return vars;
}

Coordinates Sensor::readRates() {  
  sensors_event_t g;
  m_sensor.getGyroSensor()->getEvent(&g);
  
  Coordinates rates;
  rates.x = g.gyro.y;
  rates.y = g.gyro.x;
  rates.z = g.gyro.z;
  return rates;
}

Coordinates Sensor::readAccAngles() {
  sensors_event_t a;
  m_sensor.getAccelerometerSensor()->getEvent(&a);
  
  Coordinates angles;
  angles.x = -atan(a.acceleration.x / sqrt(sq(a.acceleration.y) + sq(a.acceleration.z)));
  angles.y = atan(a.acceleration.y / sqrt(sq(a.acceleration.x) + sq(a.acceleration.z)));
  return angles;
}
