// TODO: Tune params
// TODO: Angle Controller
// TODO: Landing

#include <Adafruit_MPU6050.h>
#include <Servo.h>
#include <RF24.h>

#include "Utility.h"
#include "Receiver.h"
#include "Actuator.h"
#include "Sensor.h"
#include "Controlling.h"


Sensor* sensor;
Receiver* receiver;
IActuator* actuator;
Controlling* controlling;


void setup() {  
  Serial.begin(115200);
  Serial.println();

  receiver = new Receiver();
  receiver->setSerialOutputFlag(false);

  sensor = new Sensor();
  sensor->begin();
  sensor->calcOffsets(true, 1000);

  // Init Servos after Sensors to signal finished setup (beeping stops)
  actuator = new Servos();
  actuator->setAll(0);

  controlling = new Controlling(receiver, actuator, sensor);
  
  printAvailableMemory();
}

void loop() {
  float interval_s = getIntervalInSec(); // ~ 5,2 ms
  receiver->update(); 
  sensor->update(interval_s);
  controlling->update(interval_s);
}