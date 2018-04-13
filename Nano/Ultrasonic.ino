#include <NewPing.h>

#ifdef IS_STRIKER
#define TRIG_L 12
#define ECHO_L 11
#define TRIG_R 8
#define ECHO_R 9
#define TRIG_B -1
#define ECHO_B -1
#define TRIG_F -1
#define ECHO_F -1
#else
#define TRIG_L 7
#define ECHO_L 6
#define TRIG_R 2
#define ECHO_R 3
#define TRIG_B 8
#define ECHO_B 9
#define TRIG_F -1
#define ECHO_F -1
#endif

#define CTR_READING 81
#define NO_OBSTR_SUM 160
#define MAX_DIST 200

static NewPing sonar_l(TRIG_L, ECHO_L, MAX_DIST);
static NewPing sonar_r(TRIG_R, ECHO_R, MAX_DIST);
static NewPing sonar_b(TRIG_B, ECHO_B, MAX_DIST);
static NewPing sonar_f(TRIG_F, ECHO_F, MAX_DIST);

void InitUS() {
  pinMode(TRIG_L, OUTPUT);
  pinMode(TRIG_R, OUTPUT);
  pinMode(TRIG_B, OUTPUT);
  pinMode(TRIG_F, OUTPUT);
  pinMode(ECHO_L, INPUT);
  pinMode(ECHO_R, INPUT);
  pinMode(ECHO_B, INPUT);
  pinMode(ECHO_F, INPUT);

  Serial.println("Initialized ultrasonic.");
}

unsigned int ReadLeftUS() {
  const unsigned int reading = sonar_l.ping_cm();
#ifdef DEBUG_US
  Serial.print("Left: " + ((String) reading));
#endif
  return reading;
}

unsigned int ReadRightUS() {
  const unsigned int reading = sonar_r.ping_cm();
#ifdef DEBUG_US
  Serial.println(" Right: " + ((String) reading));
#endif
  return reading;
}

unsigned int ReadFrontUS() {
  const unsigned int reading = sonar_f.ping_cm();
#ifdef DEBUG_US
  Serial.print("Front: " + ((String) reading));
#endif
  return reading;
}

unsigned int ReadBackUS() {
  const unsigned int reading = sonar_b.ping_cm();
#ifdef DEBUG_US
  Serial.println(" Back: " + ((String) reading));
#endif
  return reading;
}

/**
 * Negative value means the bot is to the left of the center.
 * Positive value means the bot is to the right of the center.
 * Value is in centimeters.
 * 
 * The field should be 182cm in width. Since the bot is 20cm in diameter,
 * the balance reading should be 81cm on both sides.
 */
int DistFromCenterH(unsigned int left, unsigned int right) {
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
#ifdef DEBUG_US
  Serial.println("Distance: " + ((String) distance));
#endif
  return distance;
}
