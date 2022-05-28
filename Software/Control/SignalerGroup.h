#ifndef __SIGNALER_GROUP
#define __SIGNALER_GROUP

#include "Signaler.h"

class SignalerGroup : public Signaler {
private:
	Signaler* storage[MAX_SIGNAL_STORAGE];
	Vector<Signaler*> signalers;
	SignalGroup signalgroup;

public:
	SignalerGroup() : Signaler(&signalgroup) {}

	void append(Signaler* signaler) {
		signalers.push_back(signaler);
		signalgroup.append(signaler);
	}

	void remove(Signaler* signaler) {
		signalers.removeAll(signaler);
		signalgroup.remove(signaler);
	}

private:
	void onPositiveFlank() {
		Signaler::onPositiveFlank();
		for (Signaler* signaler : signalers) 
			signaler->ignoreNext();
	}
};

#endif