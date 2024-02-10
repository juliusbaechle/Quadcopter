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
  } else {
    Serial.println(F("Failed to find MPU6050 chip"));
  }
}

ProcessVars Sensor::read(float interval_s, bool& ok) {
  const float K = 0.01; // influence acc : gyro

  Coordinates rates = readRates(ok) - m_offset_rates;
  m_angles.x += interval_s * rates.x;
  m_angles.y += interval_s * rates.y;
  
  Coordinates acc_angles = readAccAngles(ok) - m_offset_angles;
  m_angles.x += K * (acc_angles.x - m_angles.x);
  m_angles.y += K * (acc_angles.y - m_angles.y);

  ProcessVars vars;
  vars.pitch = pt2_pitch(m_angles.x, interval_s);
  vars.roll = pt2_roll(m_angles.y, interval_s);
  vars.yawrate = pt2_yawrate(rates.z, interval_s);
  vars.thrust = 0;
  return vars; 
}

Coordinates Sensor::readRates(bool& ok) {  
  sensors_event_t g;
  ok &= m_sensor.getGyroSensor()->getEvent(&g);
  
  Coordinates rates;
  rates.x = g.gyro.y;
  rates.y = g.gyro.x;
  rates.z = g.gyro.z;
  
  if (isnan(rates.x) || isnan(rates.y) || isnan(rates.z)) {
    Serial.println("nan rates");    
    return Coordinates(0, 0, 0);
  }
  
  return rates;
}

Coordinates Sensor::readAccAngles(bool& ok) {
  sensors_event_t a;
  ok &= m_sensor.getAccelerometerSensor()->getEvent(&a);
  
  Coordinates angles;
  angles.x = -atan(a.acceleration.x / sqrt(sq(a.acceleration.y) + sq(a.acceleration.z)));
  angles.y = atan(a.acceleration.y / sqrt(sq(a.acceleration.x) + sq(a.acceleration.z)));
  
  if (isnan(angles.x) || isnan(angles.y)) {
    Serial.println("nan acc_angles");    
    return Coordinates(0, 0, 0);
  }
  
  return angles;
}
