int cam_x;
int cam_y;

void InitCamera() {
  Serial3.begin(BAUD_RATE);
}

void ReadCoordinates() {
  if (Serial3.available() <= 0) {
    return;
  }

  String coords[] = {};
  coords[0] = Serial3.readStringUntil(':');
  coords[1] = Serial3.readStringUntil(';');

  cam_x = coords[0].toInt();
  cam_y = coords[1].toInt();
}
