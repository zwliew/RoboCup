#include <Wire.h>

#define BAUD_RATE 9600

// Debug flags
//#define DEBUG_US

// Flags to enable/disable manually
//#define IS_STRIKER

void setup() {
  Serial.begin(BAUD_RATE);

  Wire.begin();

  InitUS();
}

void loop() {
  // Send the position to the Mega via Serial
  const int left = ReadLeftUS();
  const int right = ReadRightUS();
  const int position = CalcDistFromCenter(right, left);
  Serial.write(position);
}
