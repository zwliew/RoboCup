int cam_x;
int cam_y;

void InitCamera() {
  Serial3.begin();
}

void ReadCoordinates() {
  if (Serial3.available() <= 0) {
    return;
  }

  String coords[] = {};
  coords[0] = Serial3.readStringUntil(':');
  coords[1] = Serial3.readStringUntil(';');

  Serial.print("Coords:" + coords[0] + " ");
  Serial.println(coords[1]);

  cam_x = coords[0].toInt();
  cam_y = coords[1].toInt();

  Serial.print("Cam: " + cam_x + " ");
  Serial.println(cam_y);
}
