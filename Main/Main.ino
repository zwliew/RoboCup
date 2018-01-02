#include <Wire.h>

#define BAUD_RATE 9600

// Light sensors
#define LI_FLF A6
#define LI_FLB A7
#define LI_FRF A8
#define LI_FRB A9

#define LI_LL A3
#define LI_LM A4
#define LI_LR A15

#define LI_RL A12
#define LI_RM A11
#define LI_RR A10

#define LI_BF A2
#define LI_BM A1
#define LI_BB A0

// Photo gate sensor
#define PH_GT A15

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("Initializing.");

  Wire.begin();

  InitCmp();
}

void loop() {
  int bearing = ReadCmp();
  Serial.println(bearing);
}
