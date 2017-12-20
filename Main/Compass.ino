#include <Wire.h>

#define ADDRESS 0x60
#define HI_REG 2

int offset;

void SetupCmp() {
  offset = ReadCmp();
}

int ReadCmp() {
  Wire.beginTransmission(ADDRESS);
  Wire.write(HI_REG);
  Wire.endTransmission();
  Wire.requestFrom(ADDRESS, 2);

  char high;
  char low;
  high = Wire.read();
  low = Wire.read();

  Serial.println(high);
  Serial.println(low);

  int angle16;
  angle16 = high;
  angle16 <<= 8;
  angle16 += low;
  angle16 /= 10;

  return (angle16 - offset) % 360;
}
