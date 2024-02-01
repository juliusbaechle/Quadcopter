#pragma once

class IElement {
public:
  IElement(float a_ki) {
    m_ki = a_ki;
  }

  float operator() (float a_val, float a_intervalS) {
    if (m_ki <= 0) 
      return 0;
    m_integral += m_ki * a_val * a_intervalS;
    return m_integral;
  }

  void reset() {
    m_integral = 0;
  }

  void setKi(float Ti) { m_ki = Ti; }
  float getKi() { return m_ki; }

private:
  float m_ki = 0;
  float m_integral = 0;
};


class DT1Element {
public:
  DT1Element(float a_kd, float a_kt1)
  : kt1(a_kt1)
  , kd(a_kd) 
  {}

  float operator() (float u, float t) {
    if (kt1 <= 0 || kd <= 0)
      return 0;
    lastY = (kd * (u - lastU) - kt1 * lastY) / (kt1 + t);
    lastU = u;
    return lastY;
  }

  void reset() { 
    lastY = 0; 
    lastU = 0;
  }

private:
  float lastY = 0.0;
  float lastU = 0.0;
  float kd = 0.0;
  float kt1 = 0.0;
};


class PIDT1Element {
public:
  PIDT1Element(float a_kp, float a_ki, float a_kd)
    : m_kp(a_kp)
    , m_iElement(a_ki)
    , m_dt1Element(a_kd, a_kd / 10) {}

  float operator() (float a_val, float interval_s) {
    float val = 0.0f;
    val += m_iElement(a_val, interval_s);
    val += m_kp * a_val;
    val += m_dt1Element(a_val, interval_s);
    return val;
  }

  void reset() {
    m_iElement.reset();
    m_dt1Element.reset();
  }

private:
  IElement m_iElement;
  float m_kp = 0.0;
  DT1Element m_dt1Element;
};
