#ifndef Light_h
#define Light_h

#include "Arduino.h"

class Light {
  private:
    bool atLeftBorder();
    bool atRightBorder();
    bool atFrontBorder();
    bool atBackBorder();
  public:
    bool read(int *location);
};

#endif
