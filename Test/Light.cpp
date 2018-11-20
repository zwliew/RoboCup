/**
 * Processes readings from the light sensors for detecting the arena borders
 */
#include "Light.h"

#define ADDRESS_BACK A0
#define ADDRESS_LEFT A1
#define ADDRESS_FRONT A2
#define ADDRESS_RIGHT A3

#define LEFT_THRESHOLD 220
#define RIGHT_THRESHOLD 220
#define FRONT_THRESHOLD 220
#define BACK_THRESHOLD 700

#define FRONT_LOCATION 0
#define RIGHT_LOCATION 1
#define BACK_LOCATION 2
#define LEFT_LOCATION 3

bool Light::atLeftBorder()
{
  int reading = analogRead(ADDRESS_LEFT);
  return reading > LEFT_THRESHOLD;
}

bool Light::atRightBorder()
{
  int reading = analogRead(ADDRESS_RIGHT);
  return reading > RIGHT_THRESHOLD;
}

bool Light::atFrontBorder()
{
  int reading = analogRead(ADDRESS_FRONT);
  return reading > FRONT_THRESHOLD;
}

bool Light::atBackBorder()
{
  int reading = analogRead(ADDRESS_BACK);
  return reading > BACK_THRESHOLD;
}

bool Light::read(int *location)
{
  if (atFrontBorder()) {
    *location =  FRONT_LOCATION;
  } else if (atRightBorder()) {
    *location = RIGHT_LOCATION;
  } else if (atBackBorder()) {
    *location = BACK_LOCATION;
  } else if (atLeftBorder()) {
    *location = LEFT_LOCATION;
  }
}

