#ifndef __ANALOG_SIGNAL
#define __ANALOG_SIGNAL

#define RESOLUTION 1024

class JoystickPoti {
public:
	JoystickPoti(int h_pin) {
		pin = h_pin;
		pinMode(pin, INPUT);
		center = analogRead(pin);
	}

	uint16_t get0To1000() {
		int value = analogRead(pin);
		if (value < center) value = map(value, 0, 500, 0, 500);
		else value = map(value, center, RESOLUTION - 1, 500, 1000);
		return value;
	}

private:
	int pin;
	int center;
};

#endif