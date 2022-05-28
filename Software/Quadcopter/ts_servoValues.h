#pragma once

// Kr臟te der einzelnen Motoren in Promille [云
typedef struct s_servoValues {
  float frontRight = 0; // [云
  float backRight  = 0; // [云
  float backLeft   = 0; // [云
  float frontLeft  = 0; // [云

  struct s_servoValues operator + (struct s_servoValues servoValuess) {
    struct s_servoValues vals;
    vals.frontRight = frontRight + servoValuess.frontRight;
    vals.backRight  = backRight  + servoValuess.backRight;
    vals.backLeft   = backLeft   + servoValuess.backLeft;
    vals.frontLeft  = frontLeft  + servoValuess.frontLeft;
    return vals;
  }

  void operator += (struct s_servoValues servoValues) {
    frontRight += servoValues.frontRight;
    backRight  += servoValues.backRight;
    backLeft   += servoValues.backLeft;
    frontLeft  += servoValues.frontLeft;
  }

  void operator+= (float val) {
    frontRight += val;
    backRight += val;
    backLeft += val;
    frontLeft += val;
  }

  void operator= (float val) {
    frontRight = val;
    backRight = val;
    backLeft = val;
    frontLeft = val;
  }
} ts_servoValues;