int cam_x;
int cam_y;

#define PI_BAUD_RATE 112500

static unsigned int angle = 0;
static unsigned int distance = 0;

void InitCamera() {
  Serial1.begin(PI_BAUD_RATE);

#ifdef NO_DEBUG_OPT
  Serial.println("Initialized camera.");
#endif
}

void ReadCamera() {
  if (!Serial1.available()) {
    return;
  }
  String data = Serial1.readStringUntil(';');
  unsigned int comma_index = data.indexOf(',');
  angle = data.substring(0, comma_index).toInt();
  distance = data.substring(comma_index + 1).toInt();

#ifdef DEBUG_CAMERA
  Serial.println(((String) angle) + " " + ((String) distance));
#endif
}
