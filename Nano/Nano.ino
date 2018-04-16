#include <Wire.h>

#define BAUD_RATE 9600
#define I2C_ADDR 0x01

// Debug flags
//#define DEBUG_US

// Flags to enable/disable manually
//#define IS_STRIKER

static int distance = 0;
static unsigned int back = 0;

void setup() {
  Serial.begin(BAUD_RATE);

  InitUS();

  Wire.begin(I2C_ADDR);
  Wire.onRequest(sendDistance);
}

void loop() {
  const unsigned int left = ReadLeftUS();
  const unsigned int right = ReadRightUS();
  distance = DistFromCenterH(left, right);
  back = ReadBackUS();
}

void sendDistance() {
  Wire.write(distance > 0 ? 1 : 0);
  const unsigned int dist_mag = abs(distance);
  Wire.write(dist_mag >> 8);
  Wire.write(dist_mag % 256);
  Wire.write(back >> 8);
  Wire.write(back % 256);
}