#include <Wire.h>

#define BAUD_RATE 9600

void setup() {
  Serial.begin(BAUD_RATE);

  Serial.println("Setting up.");

  Wire.begin();

  InitGate();
  InitLoc();
  InitLight();

  Serial.println("Setup complete.");
}

void loop() {
  ReadLight();
}
