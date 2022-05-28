#ifndef __SENSITIVITY
#define __SENSITIVITY

enum Mode { EASY, MEDIUM, HARD };

class Sensitivity {
public:
	const double factor[3] = {0.33, 0.66, 1};

private:
	Mode mode = MEDIUM;

public:
	int calc(int val) {
		return val * factor[mode];
	}

	void setMode(Mode h_mode) {
		mode = h_mode;
	}
};

#endif