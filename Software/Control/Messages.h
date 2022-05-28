#pragma once

class PowerOffMessage {
public:
	static char* create() {
		static char txData[2] = { 'O', 0 };
		return txData;
	}

	static void print(char* txData, bool ack) {
		Serial.print("sent ");
		Serial.print(txData);

		if (ack) Serial.println(" ack");
		else Serial.println(" no ack");
	}
};


#include "Axis.h"

class SetpointMessage {
private:
	static const int NUM_AXES = 4;

public:
	static char* create(Axis** axes) {
		static char txData[13];
		for (int i = 0; i < NUM_AXES; i++) {
			int val = axes[i]->getValue();
			txData[3 * i + 0] = axes[i]->getId();
			txData[3 * i + 1] = highByte(val);
			txData[3 * i + 2] = lowByte(val);
		}
		txData[12] = 0;
		return txData;
	}

	static void print(char* text, bool ack) {
		Serial.print("sent ");

		for (int i = 0; i < 4; i++) {
			int16_t val = (int16_t)text[3 * i + 1] << 8;
			val |= text[3 * i + 2] & 0xFF;

			Serial.print(text[3 * i]);
			Serial.print(" ");
			Serial.print(val);
			if (i != 3) Serial.print(", ");
		}

		if (ack) Serial.println(" ack");
		else Serial.println(" no ack");
	}
};