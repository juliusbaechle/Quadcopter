/*
 Name:		Fernsteuerung.ino
 Created:	09.08.2019 - 11.08.2019
 Author:	Julius B�chle
*/

#include <RF24.h>
#include <nRF24L01.h>
#include <QArduino.h>

#include "Signaler.h"
#include "SignalerGroup.h"
#include "Axis.h"
#include "Messages.h"
#include "Transmitter.h" 

#define SENDING_INTERVAL 50
#define SERIAL_OUT false

Signaler btnLeft(new Pin(2, false));
Signaler btnRight(new Pin(3, false));
SignalerGroup btnGroup;

Sensitivity sensitivity;
Axis *throttle, *pitch, *roll, *yaw;

Transmitter* transmitter;

bool power = false;


void setup() {
  Serial.begin(115200);
  transmitter = new Transmitter();
  sensitivity.setMode(MEDIUM);

  setupButtons();
  setupAxes();

  auto timer = new Timer(SENDING_INTERVAL);
  timer->SglTimeout.connect(sendStatus);
  timer->start();
}

void setupAxes() {
  throttle = new BipolarAxis(new JoystickPoti(A0), &sensitivity, 10000, 'A'); // [milli m/s^2] -10 m/s^2 ... 10 m/s^2
  roll     = new BipolarAxis(new JoystickPoti(A3), &sensitivity,   785, 'R'); // [milli rad]   -45� ... 45�
  pitch    = new BipolarAxis(new JoystickPoti(A2), &sensitivity,   785, 'P'); // [milli rad]   -45� ... 45�
  yaw      = new BipolarAxis(new JoystickPoti(A1), &sensitivity,  6283, 'Y'); // [milli rad/s] -1 U/s ... 1 U/s
}

void setupButtons() {
  btnLeft.SglClickedOnce.connect([]() { power = true; });
  btnLeft.SglClickedLong.connect([]() { sensitivity.setMode(EASY); });
  
  btnRight.SglClickedOnce.connect([]() { power = false; });
  btnRight.SglClickedLong.connect([]() { sensitivity.setMode(MEDIUM); });
  
  btnGroup.append(&btnLeft);
  btnGroup.append(&btnRight);
  btnGroup.SglClickedOnce.connect([]() { });
  btnGroup.SglClickedLong.connect([]() { sensitivity.setMode(HARD); });
}


void sendStatus() {
  if (power) {
    Axis* axes[] = { throttle, roll, pitch, yaw };
    char* text = SetpointMessage::create(axes);
    transmitter->send(text, SERIAL_OUT);
  }
  else {
    char* text = PowerOffMessage::create();
    transmitter->send(text, SERIAL_OUT);
  }
}

void loop() {
  Scheduler::exec();
}
