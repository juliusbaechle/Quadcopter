#pragma once

#include "DigitalSignal.h"

#define DEFAULT_DEBOUNCE_TIME 100
#define REACTIVATE_TIME 1000

class Signaler : public DigitalSignal {
public:
	Signal<> SglClickedOnce;
	Signal<> SglClickedLong;

public:
	Signaler(DigitalSignal* signal, bool canBeTwice = false) {
		m_signal = signal;

		Timer* timer = new Timer(50);
		timer->SglTimeout.connect(this, SLOT() Signaler::update);
		timer->start();

		m_timer.SglTimeout.connect(this, SLOT() Signaler::timeout);
	}

	//============================================================
	// ignoreNext / ignore
	//============================================================
	// next deactivatingFlank() will be ignored
	//
	// otherways the group-elements and the 
	// group itself call their onShort/onLong functions
	//============================================================
	void ignoreNext() {
		m_ignore = true;
	}

	bool isActive() {
		return m_signal->isActive();
	}

private:
	void update() {
		if (millis() - m_lastMillis < DEFAULT_DEBOUNCE_TIME) return;

		bool active = m_signal->isActive();

		if (active != m_lastState) {
			m_lastMillis = millis();
			m_lastState = active;

			if (active) onPositiveFlank();
			else onNegativeFlank();
		}
	}

protected:
	virtual void onPositiveFlank() {
		m_timer.start();
	}

	virtual void onNegativeFlank() {
		if (!m_timer.isActive()) return;
		if (!m_ignore) SglClickedOnce.emit();
		m_ignore = false;
		m_timer.stop();
	}

private:
	void timeout() {
		if (!m_ignore) SglClickedLong.emit();
		m_ignore = false;
		m_timer.stop();
	}

private:
	DigitalSignal* m_signal;
	bool m_ignore = false;

	bool m_lastState = false;
	uint32_t m_lastMillis = millis();
	Timer m_timer{ REACTIVATE_TIME };
};