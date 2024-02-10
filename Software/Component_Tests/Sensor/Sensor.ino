#include <Adafruit_MPU6050.h>

Adafruit_MPU6050 sensor;

void setup() {
  Serial.begin(115200);
  
  bool connected = false;
  for (int i = 0; i < 3; i++) {
    connected |= sensor.begin();
    delay(200);
  }

  if (connected) {
    sensor.setAccelerometerRange(MPU6050_RANGE_4_G); // +- 4 g        (2, 4, 8, 16)
    sensor.setGyroRange(MPU6050_RANGE_500_DEG);      // +- 500 �/s    (250, 500, 1000, 2000)
    sensor.setFilterBandwidth(MPU6050_BAND_94_HZ);   // 94 Hz         (260, 184, 94, 44, 21, 10, 5)
  } else {
    Serial.println(F("Failed to find MPU6050 chip"));
  }
}

void readRates() {  
  sensors_event_t g;
  sensor.getGyroSensor()->getEvent(&g);
  
  Serial.print(g.gyro.x);
  Serial.print(",");
  Serial.print(g.gyro.y);
  Serial.print(",");
  Serial.println(g.gyro.z);
}

void readAngles() {  
  sensors_event_t a;
  sensor.getAccelerometerSensor()->getEvent(&a);

  auto angle_x = -atan(a.acceleration.x / sqrt(sq(a.acceleration.y) + sq(a.acceleration.z)));
  auto angle_y = atan(a.acceleration.y / sqrt(sq(a.acceleration.x) + sq(a.acceleration.z)));
  
  Serial.print(angle_x);
  Serial.print(",");
  Serial.println(angle_y);
}

void loop() {
  readRates();
}
