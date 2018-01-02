#include <Wire.h>

#define BAUD_RATE 9600

void setup() {
  Serial.begin(BAUD_RATE);

  Serial.println("Initializing.");

  Wire.begin();

  InitCmp();

  Serial.println("Initialized.");
}

void loop() {
  int bearing = ReadCmp();
  Serial.print("Bearing: ");
  Serial.print(bearing);
}