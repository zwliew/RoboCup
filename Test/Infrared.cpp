/**
 * Processes readings from the infrared sensor for detecting the ball
 */
#include "Infrared.h"

#define ADDRESS_BACK 0x01
#define ADDRESS_FRONT 0x02

#define BACK_ARRAY_START 7

void Infrared::init()
{
  Wire.beginTransmission(ADDRESS_FRONT);
  Wire.write(14);
  Wire.endTransmission();
  Wire.requestFrom(ADDRESS_FRONT, 1);

  Wire.beginTransmission(ADDRESS_BACK);
  Wire.write(14);
  Wire.endTransmission();
  Wire.requestFrom(ADDRESS_BACK, 1);
}

int Infrared::readFrontStrength()
{
  Wire.beginTransmission(ADDRESS_FRONT);
  Wire.write(9);
  Wire.endTransmission();
  Wire.requestFrom(ADDRESS_FRONT, 1);
  return Wire.read();
}

int Infrared::readBackStrength()
{
  Wire.beginTransmission(ADDRESS_BACK);
  Wire.write(9);
  Wire.endTransmission();
  Wire.requestFrom(ADDRESS_BACK, 1);
  return Wire.read();
}

int Infrared::readFrontDirection()
{
  Wire.beginTransmission(ADDRESS_FRONT);
  Wire.write(8);
  Wire.endTransmission();
  Wire.requestFrom(ADDRESS_FRONT, 1);
  return Wire.read();
}

int Infrared::readBackDirection()
{
  Wire.beginTransmission(ADDRESS_BACK);
  Wire.write(8);
  Wire.endTransmission();
  Wire.requestFrom(ADDRESS_BACK, 1);
  return Wire.read();
}

void Infrared::read(int *strength, int *direction)
{
  int frontStrength = readFrontStrength();
  int backStrength = readBackStrength();

  if (frontStrength > backStrength) {
    *strength = frontStrength;
    *direction = readFrontDirection();
  } else {
    *strength = readBackStrength();
    // Back sensors should range from 8 to 14
    *direction = readBackDirection() + BACK_ARRAY_START;
  }
}

