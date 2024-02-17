#include <Servo.h>

Servo servos[4];

void setup() {
  Serial.begin(115200);
  servos[0].attach(8); // FL / ESC4
  servos[1].attach(5); // FR / ESC2
  servos[2].attach(4); // RL / ESC3
  servos[3].attach(7); // RR / ESC1
  for(int i = 0; i < 4; i++)
    servos[i].writeMicroseconds(1000);
}

void loop() {
  if(Serial.available()) {
    String str = Serial.readString();
    str.trim();
    
    long percent = str.toInt();
    if (percent > 40)
      percent = 40;
    if (percent < 0)
      percent = 0;
    
    for(int i = 0; i < 4; i++)
      servos[i].writeMicroseconds(1000 + 10 * percent);
      
    Serial.print("wrote ");
    Serial.print(percent);
    Serial.println(" %");
  }
}
