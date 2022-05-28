#ifndef __SIGNAL
#define __SIGNAL

#define MAX_SIGNAL_STORAGE 20

class DigitalSignal {
public:
	virtual bool isActive() = 0;
};

class Pin : public DigitalSignal {
private:
	int m_pin;
	bool m_highActive;

public:
	Pin(int pin, bool highActive = true) {
		m_pin = pin;
		if (highActive) pinMode(m_pin, INPUT);
		else pinMode(m_pin, INPUT_PULLUP);
		m_highActive = highActive;
	}

	bool isActive() {
		return m_highActive == digitalRead(m_pin);
	}
};

class SignalGroup : public DigitalSignal {	
private:
	DigitalSignal* storage[MAX_SIGNAL_STORAGE];
	Vector<DigitalSignal*> signals;

public:
	void append(DigitalSignal* signaler) {
		signals.append(signaler);
	}

	void remove(DigitalSignal* signaler) {
		signals.removeAll(signaler);
	}

	bool isActive() {
		if (!allSignalsActive() || signals.size() == 0) return false;
		return true;
	}

private:
	bool allSignalsActive() {
		for (DigitalSignal* signal : signals) 
			if (!signal->isActive()) return false;
		return true;
	}
};

#endif