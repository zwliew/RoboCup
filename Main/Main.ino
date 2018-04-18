#include <Wire.h>

#include "Angles.h"
#include "Ultrasonic.h"

// Debug flags
//#define DEBUG_LIGHT
//#define DEBUG_COMPASS
//#define DEBUG_LOCOMOTION
//#define DEBUG_US
//#define DEBUG_CAMERA
#define NO_DEBUG_OPT
#define NO_COMPASS

// Flags to enable/disable manually
#define IS_STRIKER

#ifdef NO_DEBUG_OPT
#define BAUD_RATE 250000
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
#ifndef NO_COMPASS
  InitCmp();
#endif

#ifdef NO_DEBUG_OPT
  Serial.println("Main Setup complete.");
#endif
}

// Returns true if the loop should end early
bool debugLoop() {
  return true;
}

#ifdef IS_STRIKER
void loop() {
  if (debugLoop()) {
    return;
  }

  // If we are in possession of the ball, dribble.
  const unsigned int gate_reading = ReadGate();
  if (IsBallInGate(gate_reading)) {
    Dribble();
  }

  // Get ultrasonic distances.
  unsigned int front, left, right, back;
  ReadUltrasonic(&front, &left, &right, &back);

  // If we are out of the field,
  // move in the other direction
  unsigned int proximity = 0;
  int out_corr_dir = -1;
  const bool out[4] = {
    IsFrontOut(),
    IsLeftOut(),
    IsRightOut(),
    IsBackOut()
  };
  if (out[0]) {
    out_corr_dir = BACK_DEG;
    proximity = FindEdgeProx(front);
  } else if (out[1]) {
    out_corr_dir = RIGHT_DEG;
    proximity = FindEdgeProx(left);
  } else if (out[2]) {
    out_corr_dir = LEFT_DEG;
    proximity = FindEdgeProx(right);
  } else if (out[3]) {
    out_corr_dir = FRONT_DEG;
    proximity = FindEdgeProx(back);
  }
  if (out_corr_dir != -1) {
    Move(0.6, out_corr_dir, proximity);
    delay(350);
    return;
  }

  // If we are in possession of the ball, reposition then shoot.
  if (IsBallInGate(gate_reading)) {
    const int center_dev = AtHorizontalCenter(left, right);
    switch (center_dev) {
      case RIGHT:
        proximity = FindEdgeProx(left);
        Move(0.5, LEFT_DEG, proximity);
        break;
      case LEFT:
        proximity = FindEdgeProx(right);
        Move(0.5, RIGHT_DEG, proximity);
        break;
      default:
        StopDribble();
        Shoot();
        break;
    }
    return;
  }

  // Otherwise, track and follow the ball
  unsigned int angle, distance;
  TrackBall(&angle, &distance);
  unsigned int quadrant = CalcQuadrant(angle);
  switch (quadrant) {
    case FIRST_QUAD:
    case FOURTH_QUAD:
      proximity = FindEdgeProx(right);
      break;
    case SECOND_QUAD:
    case THIRD_QUAD:
      proximity = FindEdgeProx(left);
      break;
    default:
      proximity = FAR;
      break;
  }
  Move(0.5, angle, proximity);
}
#else
void loop() {
  if (debugLoop()) {
    return;
  }

  // Get ultrasonic distances
  unsigned int front, left, right, back;
  ReadUltrasonic(&front, &left, &right, &back);

  // If we are in possession of the ball, shoot.
  const unsigned int gate_reading = ReadGate();
  if (IsBallInGate(gate_reading)) {
    Shoot();
  }

  // Ensure the bot is within the goal area
  unsigned int proximity = 0;
  const bool in_goal = WithinGoalArea(back);
  if (!in_goal) {
    if (right > left) {
      proximity = FindEdgeProx(right);
      Move(0.4, RIGHT_DEG, proximity);
    } else {
      proximity = FindEdgeProx(left);
      Move(0.4, LEFT_DEG, proximity);
    }
    return;
  }

  // Otherwise, track and follow the ball
  unsigned int angle, distance;
  TrackBall(&angle, &distance);
  unsigned int quadrant = CalcQuadrant(angle);
  switch (quadrant) {
    case FIRST_QUAD:
    case FOURTH_QUAD:
      proximity = FindEdgeProx(right);
      Move(0.4, RIGHT_DEG, proximity);
      break;
    case SECOND_QUAD:
    case THIRD_QUAD:
      proximity = FindEdgeProx(left);
      Move(0.4, LEFT_DEG, proximity);
      break;
    default:
      break;
  }
}
#endif
