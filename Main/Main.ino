#include <Wire.h>

#define BAUD_RATE 9600

// Photo gate sensor
#define PH_GT A15

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("Initializing.");

  Wire.begin();

  SetupCmp();
}

void loop() {
  int bearing = ReadCmp();
  Serial.println(bearing);
}