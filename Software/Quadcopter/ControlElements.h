#pragma once

class PT2Element {
public:
  PT2Element(float a_kp, float a_t1, float a_t2) {
    m_kp = a_kp;
    m_t1 = a_t1;
    m_t2 = a_t2;
  }

  float operator() (float u, float t) {
    m_y1 += (m_kp * u - m_y1) * (t / (t + m_t1));
    m_y2 += (m_y1 - m_y2) * (t / (t + m_t2));
    return m_y2;
  }

  void reset() {
    m_y1 = 0;
    m_y2 = 0;
  }

private:
  float m_kp = 0;
  float m_t1 = 0;
  float m_t2 = 0;
  float m_y1 = 0;
  float m_y2 = 0;
};


class IElement {
public:
  IElement(float a_ki) {
    m_ki = a_ki;
  }

  float operator() (float u, float t) {
    if (m_ki <= 0) 
      return 0;
    m_iu += m_ki * u * t;
    return m_iu;
  }

  void reset() {
    m_iu = 0;
  }

private:
  float m_ki = 0;
  float m_iu = 0;
};


class DT2Element {
public:
  DT2Element(float a_kd, float a_kt)
  : kt(a_kt)
  , pt2(1, a_kt, a_kt) 
  {}

  float operator() (float u, float t) {
    if (kd <= 0)
      return 0;
    float y = kd * pt2(u - u1, t) / t;
    u1 = u;
    return y;
  }

  void reset() {
    pt2.reset();
    u1 = 0;
  }

private:
  float u1 = 0.0;
  float kd = 0.0;
  float kt = 0.0;
  PT2Element pt2;
};


class PIDT2Element {
public:
  PIDT2Element(float a_kp, float a_ki, float a_kd, float a_t)
    : m_kp(a_kp)
    , m_i(a_ki)
    , m_dt2(a_kd, a_t) 
  {}

  float operator() (float u, float t) {
    float val = 0.0f;
    val += m_i(u, t);
    val += m_kp * u;
    val += m_dt2(u, t);
    return val;
  }

  void reset() {
    m_i.reset();
    m_dt2.reset();
  }

private:
  IElement m_i;
  float m_kp = 0.0;
  DT2Element m_dt2;
};
