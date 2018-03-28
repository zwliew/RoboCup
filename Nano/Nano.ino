#include <Wire.h>

#define BAUD_RATE 9600

void setup() {
  Serial.begin(BAUD_RATE);

  Serial.println("Setting up Nano.");

  Wire.begin();

  // TODO: Uncomment this
  //InitCmp();
  InitUS();

  Serial.println("Nano setup complete.");
}

void loop() {
  Serial.print("Left: ");
  Serial.print(ReadLeftUS());
  Serial.print(" Right: ");
  Serial.println(ReadRightUS());
}
