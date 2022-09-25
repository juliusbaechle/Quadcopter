#include <Servo.h>
Servo servo;

// 8 VL
// 7 RR
// 5 VR
// 4 RL

void setup() {
  Serial.begin(115200);
  servo.attach(7);
  servo.writeMicroseconds(1000);
}

void loop() {
  if(Serial.available()) {
    String str = Serial.readString();
    str.trim();
    long us = str.toInt();
    servo.writeMicroseconds(us);
    Serial.print("Wrote: ");
    Serial.println(us);
  }
}
