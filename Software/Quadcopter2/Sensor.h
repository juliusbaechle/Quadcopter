#pragma once

#include <Adafruit_MPU6050.h>
#include "ProcessVars.h"

class Sensor {
public:
  void begin();
  ProcessVars getVars();

private:
  void calibrate();
  ProcessVars readSensor();

private:
  Adafruit_MPU6050 m_sensor;
  ProcessVars m_offsets;
};