#include <Wire.h>
#include <Math.h>

#include "Angles.h"
#include "Ultrasonic.h"

// Debug flags
//#define DEBUG_LIGHT
//#define DEBUG_COMPASS
//#define DEBUG_LOCOMOTION
//#define DEBUG_US
//#define DEBUG_CAMERA
#define NO_DEBUG_OPT
//#define NO_COMPASS

// Flags to enable/disable manually
#define IS_STRIKER

#ifdef NO_DEBUG_OPT
#define BAUD_RATE 9600
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
  unsigned int angle, distance;
  TrackBall(&angle, &distance);
  Move(0.3, angle, INVALID);
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
  int out_corr_x = 0;
  int out_corr_y = 0;
  const bool out[4] = {
    IsFrontOut(),
    IsLeftOut(),
    IsRightOut(),
    IsBackOut()
  };
  if (out[0]) {
    out_corr_y = 1;
  } else if (out[3]) {
    out_corr_y = -1;
  }
  if (out[1]) {
    out_corr_x = 1;
  } else if (out[2]) {
    out_corr_x = -1;
  }
  if (out_corr_x || out_corr_y) {
    float out_corr_dir = atan2(out_corr_y, out_corr_x);
    if (out_corr_dir < 0) {
      out_corr_dir = -out_corr_dir + HALF_PI;
    } else if (out_corr_dir < HALF_PI) {
      out_corr_dir = HALF_PI - out_corr_dir;
    } else {
      out_corr_dir = TWO_PI - (out_corr_dir - HALF_PI);
    }
    out_corr_dir *= RAD_TO_DEG;
    switch (CalcQuadrant(int(out_corr_dir))) {
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
    Move(0.7, out_corr_dir, proximity);
    delay(500);
    return;
  }

  // If we are in possession of the ball, reposition then shoot.
  if (IsBallInGate(gate_reading)) {
    const int ctr_dist = DistanceFromCenter(left, right);
    if (ctr_dist > 20) {
      Move(0.5, LEFT_DEG, FindEdgeProx(left));
    } else if (ctr_dist < -20) {
      Move(0.5, RIGHT_DEG, FindEdgeProx(right));
    } else {
      StopDribble();
      Move(0.5, FRONT_DEG, FAR);
      Shoot();
    }
    return;
  }

  // Otherwise, track and follow the ball
  unsigned int angle, distance;
  TrackBall(&angle, &distance);
  switch (CalcQuadrant(angle)) {
    case FIRST_QUAD:
      angle = 70;
      proximity = FindEdgeProx(right);
      break;
    case FOURTH_QUAD:
      angle = 160;
      proximity = FindEdgeProx(right);
      break;
    case SECOND_QUAD:
      angle = 290;
      proximity = FindEdgeProx(left);
      break;
    case THIRD_QUAD:
      angle = 200;
      proximity = FindEdgeProx(left);
      break;
    default:
      angle = FRONT_DEG;
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
