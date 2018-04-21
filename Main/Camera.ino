#define PI_BAUD_RATE 115200

#define TOL_DEG 20

void InitCamera() {
#ifdef IS_STRIKER
  Serial1.begin(PI_BAUD_RATE);
#else
  Serial3.begin(PI_BAUD_RATE);
#endif

#ifdef NO_DEBUG_OPT
  Serial.println("Initialized camera.");
#endif
}

void TrackBall(unsigned int *angle, float *distance) {
  static unsigned int prev_angle = NO_DEG;
  static float prev_distance = NO_DEG;
  static unsigned int no_deg_counter = 0;
  static unsigned long first_no_deg_time = 0;

#ifdef IS_STRIKER
  if (!Serial1.available()) {
#else
  if (!Serial3.available()) {
#endif
#ifdef DEBUG_CAMERA
    Serial.println("No camera reading.");
#endif
    const unsigned long cur_time = millis();
    if (cur_time > first_no_deg_time + 1000) {
      no_deg_counter = 1;
      first_no_deg_time = cur_time;
    } else {
      no_deg_counter++;
    }
    if (no_deg_counter > 10) {
      *angle = NO_DEG;
      *distance = NO_DEG;
    } else {
      *angle = prev_angle;
      *distance = prev_distance;
    }
    return;
  }

#ifdef IS_STRIKER
  const String data = Serial1.readStringUntil(';');
#else
  const String data = Serial3.readStringUntil(';');
#endif
  const unsigned int comma_index = data.indexOf(',');
  const unsigned int new_angle = data.substring(0, comma_index).toInt();
  const float new_distance = data.substring(comma_index + 1).toFloat();
  if (new_angle >= 0 && new_angle < 360
      && new_distance > 0 && new_distance < 8) {
    *angle = new_angle;
    *distance = new_distance;
    prev_angle = new_angle;
    prev_distance = new_distance;
  } else if (new_angle == NO_DEG && new_distance == NO_DEG) {
    *angle = new_angle;
    *distance = new_distance;
    prev_angle = new_angle;
    prev_distance = new_distance;
  }
#ifdef DEBUG_CAMERA
  Serial.println("Angle: " + ((String) *angle) + " Distance: " + ((String) *distance));
#endif
}

unsigned int CalcQuadrant(unsigned int angle) {
  unsigned int ret = INVALID_QUAD;
  if (angle >= FRONT_DEG + TOL_DEG && angle < RIGHT_DEG) {
    ret = FIRST_QUAD;
  } else if (angle >= RIGHT_DEG && angle < BACK_DEG) {
    ret = FOURTH_QUAD;
  } else if (angle >= BACK_DEG && angle < LEFT_DEG) {
    ret = THIRD_QUAD;
  } else if (angle < 360 - TOL_DEG && angle > FRONT_DEG + TOL_DEG) {
    ret = SECOND_QUAD;
  }
  return ret;
}
