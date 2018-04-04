#define POS_AT_EDGE 51 // todo: verify

void InitUS() {
  Serial1.begin(9600);
}

int ReadPosition() {
  int position = -1;
  if (Serial1.available()) {
    position = Serial1.read();
  }
#ifdef DEBUG_US
  Serial.println(position);
#endif
  return position;
}

// check if abs(pos) > POS_AT_EDGE
// if true, read light sensor at respective edge (right or left)
// if light sensor reading > threshold, 