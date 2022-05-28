#include "Utility.h"
#include <Arduino.h>

void printAvailableMemory() {
  int size = 2048;
  byte* buf;
  while ((buf = (byte*)malloc(--size)) == NULL);
  free(buf);

  Serial.println();
  Serial.print("Free RAM: "); Serial.print(size); Serial.println(" byte");
  Serial.println();
}

float getIntervalInSec() {
  static uint32_t m_previous_us = 0;
  uint32_t interval_us = 0;
  
  if (m_previous_us > 0) {
    interval_us = micros() - m_previous_us;
    if (interval_us == 0) interval_us = 1;
  }
  else {
    // to ignore time in setup() function
    interval_us = 1;
  }

  m_previous_us = micros();
  return (float)interval_us / 1000000;
}