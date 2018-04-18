#define PI_BAUD_RATE 9600

#define TOL_DEG 15

void InitCamera() {
  Serial1.begin(PI_BAUD_RATE);

#ifdef NO_DEBUG_OPT
  Serial.println("Initialized camera.");
#endif
}

void TrackBall(unsigned int *angle, unsigned int *distance) {
  static unsigned int prev_angle = 0;
  static unsigned int prev_distance = 0;

  if (!Serial1.available()) {
    *angle = prev_angle;
    *distance = prev_distance;
    return;
  }

  const String data = Serial1.readStringUntil(';');
  const unsigned int comma_index = data.indexOf(',');
  const unsigned int new_angle = data.substring(0, comma_index).toInt();
  const unsigned int new_distance = data.substring(comma_index + 1).toInt();
  if (new_angle >= 0 && new_angle < 360
      && new_distance > 0 && new_distance < 8) {
    *angle = new_angle;
    prev_angle = new_angle;
    *distance = new_distance;
    prev_distance = new_distance;
  }
#ifdef DEBUG_CAMERA
  Serial.println("Angle: " + ((String) prev_angle) + " Distance: " + ((String) prev_distance));
#endif
}

int CalcQuadrant(unsigned int angle) {
  int ret = INVALID_QUAD;
  if (angle >= FRONT_DEG + TOL_DEG && angle < RIGHT_DEG) {
    ret = FIRST_QUAD;
  } else if (angle >= RIGHT_DEG && angle < BACK_DEG) {
    ret = FOURTH_QUAD;
  } else if (angle >= BACK_DEG && angle < LEFT_DEG) {
    ret = THIRD_QUAD;
  } else if (angle < 360 - TOL_DEG) {
    ret = SECOND_QUAD;
  }
  return ret;
}
