#ifndef Infrared_h
#define Infrared_h

#include <Wire.h>

#include "Arduino.h"

class Infrared {
  private:
    int readFrontStrength();
    int readBackStrength();
    int readFrontDirection();
    int readBackDirection();
  public:
    void init();
    void read(int *strength, int *direction);
};

#endif
