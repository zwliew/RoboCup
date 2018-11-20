/**
 * Processes readings from the compass for detecting the bearing
 */
#include "Compass.h"

#define ADDRESS 0x60

void Compass::init()
{
  read(&_offset);
}

void Compass::read(int *bearing)
{
  byte high;
  byte low;

  Wire.beginTransmission(ADDRESS);  // Start communication with cmps03
  Wire.write(2);                    // Send the register we wish to read
  Wire.endTransmission();
  Wire.requestFrom(ADDRESS, 2);     // Request high byte

  high = Wire.read();
  low = Wire.read();

  *bearing = (((high << 8) + low) / 10 - _offset + 360) % 360;
}

