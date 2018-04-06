#define FIELD_EDGE_OFFSET 50 // todo: verify
#define CTR_TOL 5

#ifndef IS_STRIKER
#define GOAL_AREA_EDGE_OFFSET 30 // todo: verify
#endif

void InitUS() {
  Serial3.begin(BAUD_RATE);

  Serial.println("Initialized ultrasonic.");
}

/**
 * The Arduino Nano sends the position of the bot to the Mega via Serial.
 */
int ReadPosition() {
  static int position = -1;
  if (Serial3.available()) {
    position = Serial3.read();
  }
#ifdef DEBUG_US
  Serial.println(position);
#endif
  return position;
}

/**
 * 0 => bot is at center
 * 1 => bot is to the right of the center (position > 0)
 * -1 => bot is to the left of the center (position < 0)
 */
int AtCenter(int position) {
  int ret = 0;
  if (position > CTR_TOL)
    ret = 1;
  else if (position < -CTR_TOL)
    ret = -1;
  return ret;
}

/**
 * 0 => bot is within field
 * 1 => bot is to the right of field (position > 0)
 * -1 => bot is to the left of field (position < 0)
 */
int WithinField(int position) {
  int ret = 0;
  if (position >= 0 && position < FIELD_EDGE_OFFSET)
    ret = 1;
  else if (position < 0 && position > -FIELD_EDGE_OFFSET)
    ret = -1;
  return ret;
}

#ifndef IS_STRIKER
/**
 * 0 => bot is within goal area
 * 1 => bot is to the right of goal area (position > 0)
 * -1 => bot is to the left of goal area (position < 0)
 */
int WithinGoalArea(int position) {
  int ret = 0;
  if (position >= 0 && position < GOAL_AREA_EDGE_OFFSET)
    ret = 1;
  else if (position < 0 && position > -GOAL_AREA_EDGE_OFFSET)
    ret = -1;
  return ret;
}
#endif
