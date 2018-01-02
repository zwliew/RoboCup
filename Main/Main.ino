#include <Wire.h>

#define BAUD_RATE 9600

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("Initializing.");

  Wire.begin();

  InitCmp();
  InitGate();
}

void loop() {
  int bearing = ReadCmp();
  Serial.print("Bearing: ");
  Serial.print(bearing);

  int gate = ReadGate();
  Serial.print(" Gate: ");
  Serial.print(gate);
  Serial.print(" In: ");
  Serial.println(IsBallInGate());
}
