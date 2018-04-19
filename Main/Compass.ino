#define ADDR_CMPS11 0x60
#define REG_ANGLE_16 2

static int cmp_offset = 0;

void InitCmp() {
  delay(2000); // The compass takes a while to set up

  // The board should always be switched on before the motors,
  // so we should not offset the compass reading now.
  cmp_offset = ReadCmpRaw();

#ifdef NO_DEBUG_OPT
  Serial.println("Initialized compass.");
#endif
}

unsigned int ReadCmpRaw() {
  byte high;
  byte low;

  Wire.beginTransmission(ADDR_CMPS11);
  Wire.write(REG_ANGLE_16);
  Wire.endTransmission();

  Wire.requestFrom(ADDR_CMPS11, 2);

  high = Wire.read();
  low = Wire.read();

  return (720 - (((high << 8) + low) / 10) - cmp_offset) % 360;
}

// The motors will cause an offset of ~-20 degress.
// Offset the compass reading to correct for this.
unsigned int ReadCmp() {
  unsigned int offsetted = ReadCmpRaw() + 20;
  if (offsetted > 360) {
    offsetted = offsetted - 360;
  }
#ifdef DEBUG_COMPASS
  Serial.print("Compass: ");
  Serial.println(offsetted);
#endif
  return offsetted;
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
