#include <Wire.h>

// Debug flags
//#define DEBUG_LIGHT
//#define DEBUG_COMPASS
//#define DEBUG_LOCOMOTION
//#define DEBUG_US
//#define DEBUG_CAMERA
#define NO_DEBUG_OPT

// Flags to enable/disable manually
#define IS_STRIKER

#ifdef NO_DEBUG_OPT
#define BAUD_RATE 115200
#endif

void setup() {
#ifdef NO_DEBUG_OPT
  Serial.begin(BAUD_RATE);
#endif

#ifdef NO_DEBUG_OPT
  Serial.print("Setting up Main for the ");
#ifdef IS_STRIKER
  Serial.println("striker.");
#else
  Serial.println("goalkeeper.");
#endif
#endif

  Wire.begin();

  InitLoc();
  InitSld();
  InitDribbler();
  InitCamera();
  InitCmp();

#ifdef NO_DEBUG_OPT
  Serial.println("Main Setup complete.");
#endif
}

#ifdef IS_STRIKER
void loop() {
  // Out detection
  const bool out[4] = {
    IsFrontOut(),
    IsLeftOut(),
    IsRightOut(),
    IsBackOut()
  };
  int out_corr_dir = -1;
  if (out[0]) {
    out_corr_dir = 180;
  } else if (out[1]) {
    out_corr_dir = 90;
  } else if (out[2]) {
    out_corr_dir = 270;
  } else if (out[3]) {
    out_corr_dir = 0;
  }
  if (out_corr_dir != -1) {
    Move(0.6, out_corr_dir);
    delay(350);
    return;
  }

  const unsigned int gate_reading = ReadGate();
  unsigned int back = 0;
  const int position = ReadPosition(&back);
  if (IsBallInGate(position)) {
    const int at_center = AtCenter(position);
    // Dribble, reposition, then shoot
    if (at_center == 1) {
      Move(0.4, 270);
    } else if (at_center == -1) {
      Move(0.4, 90);
    } else {
      Shoot();
    }
  }
}
#else
void loop() {
  unsigned int back = 0;
  const int position = ReadPosition(&back);
  // Ensure the bot is within the goal area
  const bool in_goal= WithinGoalArea(back);
}
#endif
