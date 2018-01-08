#include <Wire.h>

#define BAUD_RATE 9600

void setup() {
  Serial.begin(BAUD_RATE);

  Serial.println("Initializing.");

  Wire.begin();

  InitGate();
  InitLoc();
  InitLight();

  Serial.println("Initialized.");
}

void loop() {
  if (IsAnyOut()) {
    Move(0, 0);
  } else {
    Move(DEBUG_SPD, 0);
  }
}
