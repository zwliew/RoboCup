#define FIELD_EDGE_OFFSET 50 // todo: verify
#define CTR_TOL 5

#ifndef IS_STRIKER
#define GOAL_AREA_EDGE_OFFSET 30 // todo: verify
#endif

// Nano comm
#define NANO_ADDR 0x01

// Edge proximity
#define EDGE_DIST_FAR 80
#define EDGE_DIST_NEAR 55

// Center deviation
#define CTR_READING 80
#define NO_OBSTR_SUM 160

// Goalie
#define GOAL_BACK_DIST 30

/**
 * The Arduino Nano sends the position of the bot to the Mega via Serial.
 */
void ReadUltrasonic(unsigned int *front, unsigned int *left,
                    unsigned int *right, unsigned int *back) {
  Wire.requestFrom(NANO_ADDR, 8);
  const byte hi_f = Wire.read();
  const byte lo_f = Wire.read();
  const byte hi_l = Wire.read();
  const byte lo_l = Wire.read();
  const byte hi_r = Wire.read();
  const byte lo_r = Wire.read();
  const byte hi_b = Wire.read();
  const byte lo_b = Wire.read();

  *front = (hi_f << 8) + lo_f;
  *left = (hi_l << 8) + lo_l;
  *right = (hi_r << 8) + lo_r;
  *back = (hi_b << 8) + lo_b;

#ifdef DEBUG_US
  Serial.println("Front: " + ((String) *front) +" Left: " + ((String) *left) +
                " Right: " + ((String) *right) + " Back: " + ((String)* back));
#endif
}

unsigned int FindEdgeProx(unsigned int distance) {
  unsigned int ret = 1;
  if (distance > EDGE_DIST_FAR) {
    ret = FAR;
  } else if (distance < EDGE_DIST_NEAR) {
    ret = NEAR;
  } else {
    ret = OK;
  }
  return ret;
}

/**
 * Negative value means the bot is to the left of the center.
 * Positive value means the bot is to the right of the center.
 * Value is in centimeters.
 * 
 * The field should be 182cm in width. Since the bot is 20cm in diameter,
 * the balance reading should be 81cm on both sides.
 */
int DistanceFromCenter(unsigned int left, unsigned int right) {
  int distance;
  if (left + right < NO_OBSTR_SUM) {
    if (right > left) {
      distance = CTR_READING - right;
    } else {
      distance = left - CTR_READING;
    }
  } else {
    distance = (left - right) / 2;
  }
#ifdef DEBUG_US
  Serial.println("Distance: " + ((String) distance));
#endif
  return distance;
}

/**
 * 0 => bot is at center
 * 1 => bot is to the right of the center (position > 0)
 * -1 => bot is to the left of the center (position < 0)
 */
int AtHorizontalCenter(unsigned int left, unsigned int right) {
  const int position = DistanceFromCenter(left, right);
  int ret = ON;
  if (position > CTR_TOL)
    ret = RIGHT;
  else if (position < -CTR_TOL)
    ret = LEFT;
  return ret;
}

/**
 * 0 => bot is within field
 * 1 => bot is to the right of field (position > 0)
 * -1 => bot is to the left of field (position < 0)
 */
int WithinField(int position) {
  int ret = ON;
  if (position >= 0 && position < FIELD_EDGE_OFFSET)
    ret = RIGHT;
  else if (position < 0 && position > -FIELD_EDGE_OFFSET)
    ret = LEFT;
  return ret;
}

bool WithinGoalArea(unsigned int back) {
  return back < GOAL_BACK_DIST;
}
