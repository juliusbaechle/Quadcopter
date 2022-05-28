#include "Controlling.h"

Controlling::Controlling(Receiver* a_receiver, IActuator* a_actuator, ISensor* a_sensor) {
  m_receiver = a_receiver;
  m_actuator = a_actuator;
  m_sensor = a_sensor;

  m_accCntrl = new AccelerationController(a_sensor);
  m_rotCntrl = new RotationController(a_sensor);

  m_pitchContext = new PitchContext(a_sensor);
  m_pitchRateCntrl = new RateController(m_pitchContext);
  m_pitchAngleCntrl = new AngleController(m_pitchContext, m_pitchRateCntrl);

  m_rollContext = new RollContext(a_sensor);
  m_rollRateCntrl = new RateController(m_rollContext);
  m_rollAngleCntrl = new AngleController(m_rollContext, m_rollRateCntrl);
}

void Controlling::update(float interval_s) {
  if      (m_state == EState::PowerOff) PowerOff();
  else if (m_state == EState::Landing)  PowerOff(); //Landing(interval_s);
  else if (m_state == EState::Flying)   Flying(interval_s);
  else PowerOff();

  m_state = transitNewState(interval_s);
}

void Controlling::PowerOff() {
  m_actuator->setAll(0);

  // Alle Regler zurücksetzen
  m_accCntrl->reset();
  m_rotCntrl->reset();
  m_rollRateCntrl->reset();
  m_rollAngleCntrl->reset();
  m_pitchRateCntrl->reset();
  m_pitchAngleCntrl->reset();
}

void Controlling::Flying(float interval_s) {
  ts_servoValues v;
  //v = 0.6 * NEWTON2US;
  v =  m_accCntrl->calculate(m_receiver->getAcc(), interval_s);
  v += m_pitchRateCntrl->calculate(m_receiver->getPitch(), interval_s);
  v += m_rollRateCntrl->calculate(m_receiver->getRoll(), interval_s);
  v += m_rotCntrl->calculate(m_receiver->getYaw(), interval_s);
  m_actuator->set(v);
}

void Controlling::Landing(float interval_s) {
  ts_servoValues v;
  v =  m_accCntrl->calculate(-0.3, interval_s);
  v += m_pitchAngleCntrl->calculate(0, interval_s);
  v += m_rollAngleCntrl->calculate(0, interval_s);  
  v += m_rotCntrl->calculate(0, interval_s);
  m_actuator->set(v);
}


EState Controlling::transitNewState(float interval_s) {
  bool powerOff = m_receiver->isPowerOff();
  bool powerFlank = m_powerOff && !powerOff;
  m_powerOff = powerOff;

  bool connected = m_receiver->isConnected();
  bool landed = isLanded(interval_s);


  switch (m_state) {
  default:
  case EState::PowerOff:
    // Nur per Flanke freischaltbar, sonst selbstständige 
    // Aktivierung nach Landung und Verbindungswiederaufbau
    if (powerFlank && connected) return EState::Flying;
    return EState::PowerOff;

  case EState::Flying:
    if (powerOff)   return EState::PowerOff;
    // if (!connected) return EState::Landing;
    if (!connected) return EState::PowerOff;
    return EState::Flying;

  case EState::Landing:
    // Falls gelandet, PowerOff !; 
    // ACHTUNG: Funktioniert nur bei kontrolliertem Flug, Winkelregelung ungetestet; Schwerer destruktiver Absturz !!!
    if (powerOff || landed) return EState::PowerOff;
    if (connected)          return EState::Flying;
    return EState::Landing;
  }
}


#define ACC_ACCURACY 0.1
#define LANDED_TIMEOUT_S 1.0

bool Controlling::isLanded(float interval_s) {  
  float acc = m_sensor->getAccZ() - m_sensor->getGravity();
  bool accZero = acc > -ACC_ACCURACY && acc < ACC_ACCURACY;

  if (!accZero) m_timeAccZero_s = 0;
  else m_timeAccZero_s += interval_s;
  return m_timeAccZero_s > LANDED_TIMEOUT_S;
}