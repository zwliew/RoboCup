#ifndef Compass_h
#define Compass_h

#include <Wire.h>

#include "Arduino.h"

class Compass {
  private:
    int _offset;
  public:
    void init();
    void read(int *bearing);
};

#endif
