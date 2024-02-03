#include <RF24.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <Servo.h>

#include "Receiver.h"
#include "Sensor.h"
#include "Controller.h"
#include "Servos.h"
 
Receiver receiver;
Sensor sensor;
Controller controller;
Servos servos;
uint64_t us = 0;

void setup() {
  Serial.begin(115200);
  receiver.begin();
  sensor.begin();
  controller.reset();
  servos.begin();
  us = micros();
}

void loop() {
  float interval_s = ((float) (micros() - us)) / 1000000;
  us = micros();

  if(receiver.isAvailable()) {
    auto desiredVars = receiver.read(interval_s);
    auto actualVars = sensor.read(interval_s);
    auto thrust = controller.calc(desiredVars - actualVars, interval_s);
    servos.set(thrust);

    Serial.print(thrust.frontLeft);
    Serial.print(",");
    Serial.print(thrust.frontRight);
    Serial.print(",");
    Serial.print(thrust.rearLeft);
    Serial.print(",");
    Serial.println(thrust.rearRight);
  } else {
    controller.reset();
    servos.set(Thrust(0));
  }

  if (interval_s < (3 / 1000000))
    Serial.println(interval_s);
}
