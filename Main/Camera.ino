int cam_x;
int cam_y;

void InitCamera() {
  Serial3.begin(BAUD_RATE);

  Serial.println("Initialized camera.");
}

int ReadBallPosition() {
  static int angle = -1;
  if (Serial3.available()) {
    angle = Serial3.read();
  }
#ifdef DEBUG_CAMERA
  Serial.println(angle);
#endif
  return angle;
}
