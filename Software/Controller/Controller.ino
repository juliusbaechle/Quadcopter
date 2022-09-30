#include <RF24_config.h>
#include <printf.h>
#include <RF24.h>
#include <nRF24L01.h>

#include "Controller.h"
#include "Transmitter.h"

Controller controller;
Transmitter transmitter;

void setup() {
  Serial.begin(115200);
  controller.begin();
  transmitter.begin();
}

void loop() {
  auto state = controller.getState();
  print(state);
  transmitter.send(state);
  delay(50);
}