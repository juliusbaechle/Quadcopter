#pragma once

#define LIMIT(val, limit) (abs(val) > limit ? (val > 0 ? limit : -limit) : val)


class StandardController {
public:
  float operator() (float val, float interval_s) { return calculate(val, interval_s); }
  virtual float calculate(float val, float interval_s) = 0;
  virtual void reset() = 0;
};


class PT1 : public StandardController {
public:
  PT1(float T) {
    m_T = T;
  }

  float calculate(float val, float interval_s) {
    if (m_T == 0) return val;
    
    m_lastVal += (val - m_lastVal) * (interval_s / m_T);
    return m_lastVal;
  }

  void reset() {
    m_lastVal = 0;
  }

  void setT(float T) { m_T = T; }
  float getT() { return m_T; }

private:
  float m_T = 0;
  float m_lastVal = 0;
};


class PPart : public StandardController {
public:
  PPart(float kp) {
    m_kp = kp;
  }

  float calculate(float val, float interval_s) {
    return m_kp * val;
  }

  void reset() {}

  void setKp(float p) { m_kp = p; }
  float getKp() { return m_kp; }

private:
  float m_kp = 0;
};


class IPart : public StandardController {
public:
  IPart(float ki, float antiWindup = 0.0f) {
    m_ki = ki;
    m_antiWindup = antiWindup;
  }

  float calculate(float val, float interval_s) {
    if (m_ki <= 0) return 0;
    m_integral += m_ki * val * interval_s;

    if (m_antiWindup > 0) 
      m_integral = LIMIT(m_integral, m_antiWindup);

    return m_integral;
  }

  void reset() {
    m_integral = 0;
  }

  void setKi(float Ti) { m_ki = Ti; }
  float getKi() { return m_ki; }

  void setAntiWindup(float antiWindup) { m_antiWindup = antiWindup; }
  float getAntiWindup() { return m_antiWindup; }

private:
  float m_ki = 0;
  float m_integral = 0;
  float m_antiWindup = 0;
};


class DPart : public StandardController {
public:
  DPart(float kd, float TFilter = 0) {
    m_kd = kd;
    m_pt1.setT(TFilter);
  }

  float calculate(float a_val, float interval_s) {
    if (m_kd <= 0) return 0;
    float val = m_pt1.calculate(a_val, interval_s); // filter with PT1
    return (a_val - val) * (m_kd / interval_s);     // diff to pt1 as output
  }

  void reset() {
    m_pt1.reset();
  }

  void setKd(float Td) { m_kd = Td; }
  float getKd() { return m_kd; }

  void setTFilter(float T) { m_pt1.setT(T); }
  float getTFilter() { return m_pt1.getT(); }

private:
  PT1 m_pt1{ 0 };
  float m_kd = 0;
};


class PIDController : public StandardController {
public:
  PIDController(float kp, float ki, float kd)
    : m_pPart(kp)
    , m_iPart(ki)
    , m_dPart(kd, 1 / (10 * kd)) {}

  float calculate(float val, float interval_s) {
    float pVal = m_pPart(val, interval_s);
    float iVal = m_iPart(val, interval_s);
    float dVal = m_dPart(val, interval_s);
    return pVal + iVal + dVal;
  }

  void reset() {
    m_pPart.reset();
    m_iPart.reset();
    m_dPart.reset();
  }

  void setAntiWindup(float antiWindup) {
    m_iPart.setAntiWindup(antiWindup);
  }

  void setTFilter(float tFilter) {
    m_dPart.setTFilter(tFilter);
  }

private:
  DPart m_dPart;
  IPart m_iPart;
  PPart m_pPart;
};

class PDController : public StandardController {
public:
  PDController(float kp, float kd)
    : m_pPart(kp)
    , m_dPart(kd, 1 / (10 * kd)) {}

  float calculate(float val, float interval_s) {
    float pVal = m_pPart(val, interval_s);
    float dVal = m_dPart(val, interval_s);
    return pVal + dVal;
  }

  void reset() {
    m_pPart.reset();
    m_dPart.reset();
  }

  void setTFilter(float tFilter) {
    m_dPart.setTFilter(tFilter);
  }

private:
  DPart m_dPart;
  PPart m_pPart;
};

class PIController : public StandardController {
public:
  PIController(float kp, float ki)
    : m_pPart(kp)
    , m_iPart(ki) {}

  float calculate(float val, float interval_s) {
    float pVal = m_pPart(val, interval_s);
    float iVal = m_iPart(val, interval_s);
    return pVal + iVal;
  }

  void reset() {
    m_pPart.reset();
    m_iPart.reset();
  }

  void setAntiWindup(float antiWindup) {
    m_iPart.setAntiWindup(antiWindup);
  }

private:
  IPart m_iPart;
  PPart m_pPart;
};