#include <Wire.h>

#define BAUD_RATE 9600

// Debug
//#define DEBUG_US

// Flags to enable/disable manually
//#define IS_STRIKER

void setup() {
  Serial.begin(BAUD_RATE);

  Wire.begin();

  InitUS();

  Serial.println("Nano setup complete.");
}

void loop() {
  const int position = CalcDistFromCenter();
  Serial.write(position);
}
