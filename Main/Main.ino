#include <Wire.h>
#include <Math.h>

#include "Angles.h"
#include "Ultrasonic.h"

// Debug flags
//#define DEBUG_LIGHT
//#define DEBUG_COMPASS
//#define DEBUG_LOCOMOTION
//#define DEBUG_US
#define DEBUG_CAMERA
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

// Returns true if the loop should end early.
// Meant for debugging.
bool debugLoop() {
  return false;
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
  //unsigned int proximity = INVALID;
  int out_corr_x = 0;
  int out_corr_y = 0;
  const bool out[4] = {
    IsFrontOut(),
    IsLeftOut(),
    IsRightOut(),
    IsBackOut()
  };
  if (out[0]) {
    out_corr_y = -1;
  } else if (out[3]) {
    out_corr_y = 1;
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
        //proximity = FindEdgeProx(right);
        break;
      case SECOND_QUAD:
      case THIRD_QUAD:
        //proximity = FindEdgeProx(left);
        break;
      default:
        //proximity = FAR;
        break;
    }
    Move(0.4, out_corr_dir, INVALID);
    delay(500);
    return;
  }

  // If we are in possession of the ball, reposition then shoot.
  if (IsBallInGate(gate_reading)) {
    const int ctr_dist = DistanceFromCenter(left, right);
    if (ctr_dist > 20) {
      Move(0.4, LEFT_DEG, INVALID);
    } else if (ctr_dist < -20) {
      Move(0.4, RIGHT_DEG, INVALID);
    } else {
      StopDribble();
      Move(0.4, FRONT_DEG, FAR);
      Shoot();
    }
    return;
  }

  // Otherwise, track and follow the ball
  unsigned int angle;
  float distance;
  TrackBall(&angle, &distance);
  if (angle == NO_DEG && distance == NO_DEG) {
    Move(0, FRONT_DEG, INVALID);
  } else {
    switch (CalcQuadrant(angle)) {
      case FIRST_QUAD:
      case FOURTH_QUAD:
        angle += 15;
        //proximity = FindEdgeProx(right);
        break;
      case SECOND_QUAD:
      case THIRD_QUAD:
        angle -= 15;
        //proximity = FindEdgeProx(left);
        break;
      default:
        angle = FRONT_DEG;
        //proximity = INVALID;
        break;
    }
    Move(0.4, angle, INVALID);
  }
}
#else
void loop() {
  if (debugLoop()) {
    return;
  }
  // (0, 115) to (45, 0)

  // Dribble all the time to mess with the ball
  Dribble();

  // Get ultrasonic distances
  unsigned int front, left, right, back;
  ReadUltrasonic(&front, &left, &right, &back);

  // If we are in possession of the ball, shoot.
  const unsigned int gate_reading = ReadGate();
  if (IsBallInGate(gate_reading)) {
    Shoot();
  }

  // If we are out of the field,
  // move in the other direction
  //unsigned int proximity = INVALID;
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
        //proximity = FindEdgeProx(right);
        break;
      case SECOND_QUAD:
      case THIRD_QUAD:
        //proximity = FindEdgeProx(left);
        break;
      default:
        //proximity = FAR;
        break;
    }
    Move(0.4, out_corr_dir, INVALID);
    delay(500);
    return;
  }

  // Ensure the bot is within the goal area
  const bool in_goal = WithinGoalieArea(back);
  if (!in_goal) {
    if (right > left) {
      //proximity = FindEdgeProx(right);
      Move(0.4, RIGHT_BACK_DEG, INVALID);
    } else {
      //proximity = FindEdgeProx(left);
      Move(0.4, LEFT_BACK_DEG, INVALID);
    }
    return;
  }

  // Otherwise, track and follow the ball
  unsigned int angle;
  float distance;
  TrackBall(&angle, &distance);
  if (angle == NO_DEG && distance == NO_DEG) {
    const int ctr_dist = DistanceFromCenter(let, right);
    unsigned int to_base_deg = NO_DEG;
    if (back >= 60) {
      if (ctr_dist > 10) {
        to_base_deg = LEFT_BACK_DEG;
      } else if (ctr_dist < -10) {
        to_base_deg = RIGHT_BACK_DEG;
      }
    } else {
      if (ctr_dist > 15) {
        to_base_deg = LEFT_DEG;
      } else if (ctr_dist < -15) {
        to_base_deg = RIGHT_DEG;
      }
    }
    Move(to_base_deg == NO_DEG ? 0 : 0.4, to_base_deg, INVALID);
  } else {
    unsigned int quadrant = CalcQuadrant(angle);
    switch (quadrant) {
      case FIRST_QUAD:
      case FOURTH_QUAD:
        angle += 15;
        //proximity = FindEdgeProx(right);
        Move(0.4, angle, INVALID);
        break;
      case SECOND_QUAD:
      case THIRD_QUAD:
        angle -= 15;
        //proximity = FindEdgeProx(left);
        Move(0.4, angle, INVALID);
        break;
      default:
        break;
    }
  }
}
#endif
