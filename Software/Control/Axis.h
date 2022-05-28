#pragma once

#include "JoystickPoti.h"
#include "Sensitivity.h"

class Axis {

protected:
	JoystickPoti* m_joystick;
	Sensitivity* m_sensitivity;
	int center = 0;

private:
	char m_id;

public:
	Axis(JoystickPoti* joystick, Sensitivity* sensitivity, char id) {
		m_joystick = joystick;
		m_sensitivity = sensitivity;
		m_id = id;
	}

	void calibrate() {
		center += getValue() / 100;
	}

	int getValue() {
		int value = m_joystick->get0To1000();
		value = scale(value);
		value = m_sensitivity->calc(value);
		return value + center;
	}

	char getId() { return m_id; }

private:
	virtual int scale(int analogPromille) = 0;
};


class UnipolarAxis : public Axis {
private: 
	int m_maxVal;

public:
	UnipolarAxis(JoystickPoti* joystick, Sensitivity* sensitivity, int maxVal, char id) 
	: Axis(joystick, sensitivity, id) {
		m_maxVal = maxVal;
	}

private:
	int scale(int analogPromille) {
		if (analogPromille < 500) 
			return map(analogPromille, 500, 0, 0, -center);
		else 
			return map(analogPromille, 500, 1000, 0, m_maxVal - center);
	}
};


class BipolarAxis : public Axis {
private:
	long m_maxVal;

public:
	BipolarAxis(JoystickPoti* joystick, Sensitivity* sensitivity, long maxVal, char id)
	: Axis(joystick, sensitivity, id) {
		m_maxVal = maxVal;
		calibrate();
	}

private:
	int scale(int analogPromille) {
		return map(analogPromille, 0, 1000, -m_maxVal + center, m_maxVal + center);
	}
};