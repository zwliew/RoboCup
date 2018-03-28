#include <Wire.h>

#define BAUD_RATE 9600

#define DEBUG_LIGHT

void setup() {
  Serial.begin(BAUD_RATE);

  Serial.println("Setting up Main.");

  Wire.begin();

  InitGate();
  InitLoc();
  InitLight();
  InitSld();

  Serial.println("Main Setup complete.");
}

void loop() {
}
