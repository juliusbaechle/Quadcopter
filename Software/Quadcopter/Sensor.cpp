#include "Sensor.h"
#include <Adafruit_Sensor.h>

#define sqr(a) ((a)*(a))

typedef struct s_gyro {
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;
} ts_gyro;

typedef struct s_acc {
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;
} ts_acc;


bool Sensor::begin() {
  bool connected = false;
  for (int i = 0; i < 3; i++) {
    connected |= mpu.begin();
    delay(200);
  }

  if (connected) {    
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G); // +- 4 g        (2, 4, 8, 16)
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);      // +- 500 °/s    (250, 500, 1000, 2000)
    mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);    // 5 Hz          (260, 184, 94, 44, 21, 10, 5)
  } else {
    Serial.println(F("Failed to find MPU6050 chip"));
  }
}


void Sensor::calcOffsets(bool console, uint32_t samples) {
  if (console) {
    Serial.println();
    Serial.println(F("Calculating Offsets!"));
  }

  float gx = 0, gy = 0, gz = 0;
  float ax = 0, ay = 0, az = 0;

  for (int i = 0; i < samples; i++) {
    if (console && i % (samples / 3) == 0) Serial.print(".");

    sensors_event_t g;
    mpu.getGyroSensor()->getEvent(&g);
    gx += g.gyro.x;
    gy += g.gyro.y;
    gz += g.gyro.z;

    sensors_event_t a;
    mpu.getAccelerometerSensor()->getEvent(&a);
    ax += a.acceleration.x;
    ay += a.acceleration.y;
    az += a.acceleration.z;
  }

  gyroXoffset = gx / samples;
  gyroYoffset = gy / samples;
  gyroZoffset = gz / samples;

  float accX = ax / samples;
  float accY = ay / samples;
  float accZ = az / samples;

  angleXoffset = -atan2(accY, sqrt(accZ * accZ + accX * accX));
  angleYoffset = -atan2(accX, sqrt(accZ * accZ + accY * accY));
  gravity      = -sin(angleYoffset) * accX - sin(angleXoffset) * accY + cos(angleXoffset) * cos(angleYoffset) * accZ;
  accZoffset   = accZ;

  if (console) outputOffsets();
}

void Sensor::outputOffsets() {
  Serial.println();
  Serial.print(F("Gyro Offset [rad/s]: "));
  Serial.print(F("X : "));   Serial.print(gyroXoffset);
  Serial.print(F(", Y : ")); Serial.print(gyroYoffset);
  Serial.print(F(", Z : ")); Serial.println(gyroZoffset);

  Serial.print(F("Angle Offset [deg]: "));
  Serial.print(F("X : "));   Serial.print(angleXoffset * RAD2DEG);
  Serial.print(F(", Y : ")); Serial.println(angleYoffset * RAD2DEG);

  Serial.print(F("Gravity [m/s^2]: "));
  Serial.println(gravity);
  Serial.println();
}


void Sensor::update(float interval_s) {
  measure();
  calcAngle(interval_s);
  calcAcc(interval_s);
}

void Sensor::measure() {
  sensors_event_t a, g, t;
  mpu.getEvent(&a, &g, &t);

  msrdAccX = a.acceleration.x;
  msrdAccY = a.acceleration.y;
  msrdAccZ = a.acceleration.z;
  
  msrdGyroX = -(g.gyro.x - gyroXoffset);
  msrdGyroY = g.gyro.y - gyroYoffset;
  msrdGyroZ = g.gyro.z - gyroZoffset;

  msrdTemp = t.temperature;
}

void Sensor::calcAngle(float interval_s) {
  float factorAcc =  exp(-2000 * sqr(msrdAccZ - accZoffset) * sqrt(sqr(msrdAccX) + sqr(msrdAccY)));
  if (factorAcc > 1) factorAcc = 1;


  float angleAccX = -atan2(msrdAccY, sqrt(sqr(msrdAccZ) + sqr(msrdAccX)));
  float angleGyroX = angleX + msrdGyroX * interval_s;
  angleX = (1.0f - factorAcc) * angleGyroX + factorAcc * angleAccX;
  
  float angleAccY = -atan2(msrdAccX, sqrt(sqr(msrdAccZ) + sqr(msrdAccY)));
  float angleGyroY = angleY + msrdGyroY * interval_s; 
  angleY = (1.0f - factorAcc) * angleGyroY + factorAcc * angleAccY;
  
  float angleGyroZ = angleZ + msrdGyroZ * interval_s;
  angleZ = angleGyroZ;
}

void Sensor::calcAcc(float interval_s) {
  accX = cos(angleY) * msrdAccX + sin(angleY) * msrdAccZ;
  accY = cos(angleX) * msrdAccY + sin(angleX) * msrdAccZ;
  accZ = cos(sqrt(sqr(angleY) + sqr(angleX))) * msrdAccZ - sin(angleY) * msrdAccX - sin(angleX) * msrdAccY;
}