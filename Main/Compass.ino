#include <Wire.h>

#define ADDR_CMPS11 0x60
#define REG_ANGLE_16 2

static int cmp_offset = 0;

void InitCmp() {
  cmp_offset = ReadCmp();
}

int ReadCmp() {
  byte high;
  byte low;

  Wire.beginTransmission(ADDR_CMPS11);
  Wire.write(REG_ANGLE_16);
  Wire.endTransmission();

  Wire.requestFrom(ADDR_CMPS11, 2);

  high = Wire.read();
  low = Wire.read();
  return (((high << 8) + low) / 10 - cmp_offset + 360) % 360;
}
