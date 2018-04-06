#include <Wire.h>

#define BAUD_RATE 9600

// Debug flags
//#define DEBUG_LIGHT
//#define DEBUG_COMPASS
//#define DEBUG_LOCOMOTION
#define DEBUG_US
//#define DEBUG_CAMERA
#define DEBUG_MAIN

// Flags to enable/disable manually
//#define IS_STRIKER

void setup() {
  Serial.begin(BAUD_RATE);

  Serial.print("Setting up Main for the ");
#ifdef IS_STRIKER
  Serial.println("striker.");
#else
  Serial.println("goalkeeper.");
#endif

  Wire.begin();

  InitGate();
  InitLoc();
  InitLight();
  InitSld();
  InitCmp();
  InitUS();
  InitCamera();

  Serial.println("Main Setup complete.");
}

void loop() {
  int pos = ReadPosition();
#ifdef DEBUG_MAIN
  return;
#endif

  const int position = ReadPosition();
  // Ensure bot is within the field boundaries
  const int within_field = WithinField(position);
  if (within_field == 1) {
    Move(0.4, 270);
  } else if (within_field == -1) {
    Move(0.4, 90);
  } else {
    // Move according to ball position
  }

#ifdef IS_STRIKER
  const int gate_reading = ReadGate();
  if (IsBallInGate()) {
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
#else
  // Ensure the bot is within the goal area
  const int within_goal_area = WithinGoalArea(position);
  if (within_goal_area == 1) {
    Move(0.4, 270);
  } else if (within_goal_area == -1) {
    Move(0.4, 90);
  } else {
    // Move according to ball position
  }
#endif
}
