#include <Wire.h>

#define I2C_ADDR 0x01

// Debug flags
//#define DEBUG_US
//#define NO_DEBUG_OPT

#ifdef NO_DEBUG_OPT
#define BAUD_RATE 9600
#endif

// Flags to enable/disable manually
#define IS_STRIKER

static unsigned int front = 0;
static unsigned int back = 0;
static unsigned int left = 0;
static unsigned int right = 0;

void setup() {
#ifdef NO_DEBUG_OPT
  Serial.begin(BAUD_RATE);
#endif

  InitUS();

  Wire.begin(I2C_ADDR);
  Wire.onRequest(sendDistance);
}

void loop() {
  front = ReadFrontUS();
  left = ReadLeftUS();
  right = ReadRightUS();
  back = ReadBackUS();
}

void sendDistance() {
  Wire.write(front >> 8);
  Wire.write(front % 256);
  Wire.write(left >> 8);
  Wire.write(left % 256);
  Wire.write(right >> 8);
  Wire.write(right % 256);
  Wire.write(back >> 8);
  Wire.write(back % 256);
}
