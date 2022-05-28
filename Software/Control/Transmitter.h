#pragma once

#define CE_PIN   9
#define CSN_PIN 10

class Transmitter {
private:
	const byte slaveAddress[5] = {'D', 'r', 'o', 'n', 'e'};   //RxAAA
	RF24 radio;

public:
  Transmitter() : radio{CE_PIN, CSN_PIN} {
    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.setRetries(3, 5);
    radio.openWritingPipe(slaveAddress);
    radio.setPALevel(RF24_PA_MAX);
  }

  bool send(char* msg, bool serialOut) {
    bool ack = radio.write(msg, 12);

    if (serialOut) {
      if (msg[0] == 'O') 
        PowerOffMessage::print(msg, ack);
      else 
        SetpointMessage::print(msg, ack);
    }    
    
    return ack;
  }
};