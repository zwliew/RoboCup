#include <Wire.h>

#define BAUD_RATE 9600

// Debug flags
//#define DEBUG_LIGHT
//#define DEBUG_COMPASS
//#define DEBUG_LOCOMOTION
//#define DEBUG_US
//#define DEBUG_CAMERA
#define NO_DEBUG_OPT

// Flags to enable/disable manually
#define IS_STRIKER

void setup() {
  Serial.begin(BAUD_RATE);

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
  ReadCamera();
  return;

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
    Move(0.2, out_corr_dir);
    return;
  }

  const int position = ReadPosition();
  // Ensure bot is within the field boundaries
  const int within_field = WithinField(position);
  if (within_field == 1) {
    Move(0.2, 270);
  } else if (within_field == -1) {
    Move(0.2, 90);
  } else {
    // Move according to ball position
    Move(0.2, 0);
  }

  const int gate_reading = ReadGate();
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
  const int position = ReadPosition();
  // Ensure the bot is within the goal area
  const int within_goal_area = WithinGoalArea(position);
  if (within_goal_area == 1) {
    Move(0.4, 270);
  } else if (within_goal_area == -1) {
    Move(0.4, 90);
  } else {
    // Move according to ball position
  }
  Move(0, 0);
}
#endif
