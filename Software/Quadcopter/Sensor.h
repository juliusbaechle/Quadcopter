#pragma once

#include <Adafruit_MPU6050.h>
#include "ISensor.h"

class Sensor : public ISensor {
public:
  Sensor() {}

  bool begin();
  void update(float interval_s);
  void calcOffsets(bool console = false, uint32_t samples = 3000);

public: // Getters
  float getTemp() { return msrdTemp; };

  float getAccX() { return accX; };
  float getAccY() { return accY; };
  float getAccZ() { return accZ; };
  float getGravity() { return gravity; }

  float getRateX() { return msrdGyroX; };
  float getRateY() { return msrdGyroY; };
  float getRateZ() { return msrdGyroZ; };

  float getAngleX() { return angleX - angleXoffset; };
  float getAngleY() { return angleY - angleYoffset; };
  float getAngleZ() { return angleZ; };

private:
  void outputOffsets();
  void measure();
  void calcAngle(float interval_s);
  void calcAcc(float interval_s);

private:
  Adafruit_MPU6050 mpu;

  // Offsets
  float gyroXoffset = 0, gyroYoffset = 0, gyroZoffset = 0;
  float angleXoffset = 0, angleYoffset = 0;
  float gravity = 0, accZoffset = 0;

  // MeasuredValues
  float msrdTemp = 0;
  float msrdAccX = 0, msrdAccY = 0, msrdAccZ = 0;
  float msrdGyroX = 0, msrdGyroY = 0, msrdGyroZ = 0;

  // Calculated Values
  float accX, accY, accZ;
  float angleX = 0, angleY = 0, angleZ = 0;
};