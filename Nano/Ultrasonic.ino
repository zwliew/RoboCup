#include <NewPing.h>

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

#define CTR_READING 81
#define NO_OBSTR_SUM 160
#define MAX_DIST 200

static NewPing sonar_l(TRIG_L, ECHO_L, MAX_DIST);
static NewPing sonar_r(TRIG_R, ECHO_R, MAX_DIST);

void InitUS() {
  pinMode(TRIG_L, OUTPUT);
  pinMode(TRIG_R, OUTPUT);
  pinMode(ECHO_L, INPUT);
  pinMode(ECHO_R, INPUT);
}

int ReadLeftUS() {
  const int reading = sonar_l.ping_cm();
#ifdef DEBUG_US
  Serial.print("Left: " + ((String) reading));
#endif
  return reading;
}

int ReadRightUS() {
  const int reading = sonar_r.ping_cm();
#ifdef DEBUG_US
  Serial.println(" Right: " + ((String) reading));
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
int CalcDistFromCenter(int right, int left) {
  int distance;
  if (left + right < NO_OBSTR_SUM) {
    if (right > left) {
      distance = CTR_READING - right;
    } else {
      distance = left - CTR_READING;
    }
  } else {
    distance = (left - right) / 2;
  }
  return distance;
}
