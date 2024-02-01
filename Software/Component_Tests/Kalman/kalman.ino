#include <Adafruit_MPU6050.h>
#include "Sensor.h"

Sensor sensor;

void setup() {
  Serial.begin(115200);
  sensor.begin();
}

void loop() {
  auto vars = sensor.read();
    
  Serial.print((180/PI) * vars.roll);
  Serial.print(",");
  Serial.println((180/PI) * vars.pitch);
}
