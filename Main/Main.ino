#include <Wire.h>

#define BAUD_RATE 9600

// Debug flags
//#define DEBUG_LIGHT
//#define DEBUG_COMPASS
//#define DEBUG_LOCOMOTION
#define DEBUG_US
//#define DEBUG_CAMERA

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

#ifdef IS_STRIKER
void loop() {
  // Out detection
  static bool was_front_out = false;
  static bool was_back_out = false;
  static bool was_right_out = false;
  static bool was_left_out = false;
  ReadLight();
  if (IsRightOut() && !was_right_out) {
    was_right_out = true;
    Move(0.4, 270);
    return;
  } else if (IsRightOut() && was_right_out) {
    was_right_out = false;
  } else if (was_right_out) {
    Move(0.4, 270);
    return;
  }
  if (IsLeftOut() && !was_left_out) {
    was_left_out = true;
    Move(0.4, 90);
    return;
  } else if (IsLeftOut() && was_left_out) {
    was_left_out = false;
  } else if (was_left_out) {
    Move(0.4, 90);
    return;
  }
  if (IsFrontOut() && !was_front_out) {
    was_front_out = true;
    Move(0.4, 180);
    return;
  } else if (IsFrontOut() && was_front_out) {
    was_front_out = false;
  } else if (was_front_out) {
    Move(0.4, 180);
    return;
  }
  if (IsBackOut() && !was_back_out) {
    was_back_out = true;
    Move(0.4, 0);
    return;
  } else if (IsBackOut() && was_back_out) {
    was_back_out = false;
  } else if (was_back_out) {
    Move(0.4, 0);
    return;
  }

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
}
#endif
