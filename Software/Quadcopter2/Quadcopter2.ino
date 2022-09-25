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

void setup() {
  Serial.begin(115200);
  servos.begin();
  receiver.begin();
  sensor.begin();
  controller.reset();
}

void loop() {
  receiver.update();
  if(receiver.isAvailable()) {
    auto desiredVars = receiver.getVars();
    auto actualVars = sensor.getVars();
    auto thrust = controller.calc(desiredVars - actualVars);
    servos.set(thrust);
  } else {
    controller.reset();
    servos.set(Thrust(0));
  }
}