#ifdef IS_STRIKER
#define TRIG_L 12
#define ECHO_L 11
#define TRIG_R 8
#define ECHO_R 9
#else
#define TRIG_L 7
#define ECHO_L 6
#define TRIG_R 2
#define ECHO_R 3
#define TRIG_B 8
#define ECHO_B 9
#endif

void InitUS() {
  pinMode(TRIG_L, OUTPUT);
  pinMode(TRIG_R, OUTPUT);
  pinMode(ECHO_L, INPUT);
  pinMode(ECHO_R, INPUT);
}

int ReadLeftUS() {
  digitalWrite(TRIG_L, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_L, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_L, LOW);

  const int reading = pulseIn(ECHO_L, HIGH) * 0.034 / 2;
#ifdef DEBUG_US
  Serial.println("Left: " + ((String) reading));
#endif
  return reading;
}

int ReadRightUS() {
  digitalWrite(TRIG_R, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_R, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_R, LOW);

  const int reading = pulseIn(ECHO_R, HIGH) * 0.034 / 2;
#ifdef DEBUG_US
  Serial.println("Right: " + ((String) reading));
#endif
  return reading;
}

/**
 * negative value means the bot is to the left of the center
 * positive value means the bot is to the right of the center
 * value is in centimeters.
 * 
 * The field should be 182cm in width. Since the bot is 20cm in diameter,
 * the balance reading should be 81cm on both sides.
 */
int CalcDistFromCenter() {
  int left = ReadLeftUS();
  int right = ReadRightUS();

  int distance;
  if (left + right < 160) {
    if (right > left) {
      distance = 81 - right;
    } else {
      distance = left - 81;
    }
  } else {
    distance = left - right;
  }
  return distance;
}
