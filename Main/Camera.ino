int cam_x;
int cam_y;

#define PI_BAUD_RATE 230400

static unsigned int angle = 0;
static unsigned int distance = 0;

void InitCamera() {
  Serial1.begin(PI_BAUD_RATE);

#ifdef NO_DEBUG_OPT
  Serial.println("Initialized camera.");
#endif
}

unsigned int ReadCamera() {
  if (!Serial1.available()) {
    return angle;
  }
  String data = Serial1.readStringUntil(';');
  unsigned int comma_index = data.indexOf(',');
  unsigned int new_angle = data.substring(0, comma_index).toInt();
  unsigned int new_distance = data.substring(comma_index + 1).toInt();
  if (new_angle >= 0 && new_angle < 360
      && new_distance > 0 && new_distance < 10) {
    angle = new_angle;
    distance = new_distance;
  }
#ifdef DEBUG_CAMERA
  Serial.println(((String) angle) + " " + ((String) distance));
#endif
  return angle;
}
