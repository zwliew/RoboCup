  #define ADDR_CMPS11 0x60
#define REG_ANGLE_16 2

int cmp_offset = 0;

void InitCmp() {
  delay(2000); // The compass takes a while to set up
  cmp_offset = ReadCmp();

  Serial.println("Initialized compass.");
}

int ReadCmp() {
  byte high;
  byte low;

  Wire.beginTransmission(ADDR_CMPS11);
  Wire.write(REG_ANGLE_16);
  Wire.endTransmission();

  Wire.requestFrom(ADDR_CMPS11, 2);
  while (Wire.available() < 2);

  high = Wire.read();
  low = Wire.read();

  int ret = (((high << 8) + low) / 10 - cmp_offset + 360) % 360;
#ifdef DEBUG_COMPASS
  Serial.print("Compass: ");
  Serial.println(ret);
#endif
  return ret;
}

#ifdef DEBUG_COMPASS
void SpinToZeroBearing() {
  int compass = ReadCmp();
  Serial.println(compass);
  if (compass < 359 && compass > 180) {
    Spin(0.3, true);
  } else if (compass > 1 && compass < 180) {
    Spin(0.3, false);
  } else {
    Spin(0, false);
  }
}
#endif
