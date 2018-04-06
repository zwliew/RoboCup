#define POS_AT_EDGE 51 // todo: verify

void InitUS() {
  Serial3.begin(9600);
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

// check if abs(pos) > POS_AT_EDGE
// if true, read light sensor at respective edge (right or left)
// if light sensor reading > threshold, 