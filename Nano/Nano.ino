#include <Wire.h>

#define BAUD_RATE 9600

// Flags to enable/disable manually
//#define IS_STRIKER

void setup() {
  Serial.begin(BAUD_RATE);

  Serial.print("Setting up Nano for the ");
#ifdef IS_STRIKER
  Serial.println("striker.");
#else
  Serial.println("goalkeeper.");
#endif

  Wire.begin();

  InitUS();

  Serial.println("Nano setup complete.");
}

void loop() {
  Serial.print("Left: ");
  Serial.print(ReadLeftUS());
  Serial.print(" Right: ");
  Serial.println(ReadRightUS());
}
