#pragma once

#define DEG2RAD (PI/180.0)
#define RAD2DEG (180.0/PI)

class IGyrosensor {
public:
  // X angular motion [rad/s]
  virtual float getRateX() = 0;
  // Y angular motion [rad/s]
  virtual float getRateY() = 0;
  // Z angular motion [rad/s]
  virtual float getRateZ() = 0;
};

class IAngleSensor {
public:
  // X angle [rad]
  virtual float getAngleX() = 0;
  // Y angle [rad]
  virtual float getAngleY() = 0;
  // Z angle [rad]
  virtual float getAngleZ() = 0;
};

class IAccelerationSensor {
public:
  // X acceleration [m/s^2]
  virtual float getAccX() = 0;
  // Y acceleration [m/s^2]
  virtual float getAccY() = 0;
  // Z acceleration [m/s^2]
  virtual float getAccZ() = 0;
  // Z acceleration caused by gravity [m/s^2]
  virtual float getGravity() = 0;
}; 

class ISensor : public IAccelerationSensor, public IAngleSensor, public IGyrosensor {};