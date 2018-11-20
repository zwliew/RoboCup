#include <NewPing.h>

#ifdef IS_STRIKER
#define TRIG_L 6
#define ECHO_L 7
#define TRIG_R 12
#define ECHO_R 11
#define TRIG_B 8
#define ECHO_B 9
#else
#define TRIG_L 12
#define ECHO_L 11
#define TRIG_R 8
#define ECHO_R 9
#define TRIG_B 4
#define ECHO_B 5
#endif

#define MAX_DIST 200

static NewPing sonar_l(TRIG_L, ECHO_L, MAX_DIST);
static NewPing sonar_r(TRIG_R, ECHO_R, MAX_DIST);
static NewPing sonar_b(TRIG_B, ECHO_B, MAX_DIST);
//static NewPing sonar_f(TRIG_F, ECHO_F, MAX_DIST);

void InitUS() {
  pinMode(TRIG_L, OUTPUT);
  pinMode(TRIG_R, OUTPUT);
  pinMode(TRIG_B, OUTPUT);
  //pinMode(TRIG_F, OUTPUT);
  pinMode(ECHO_L, INPUT);
  pinMode(ECHO_R, INPUT);
  pinMode(ECHO_B, INPUT);
  //pinMode(ECHO_F, INPUT);

#ifdef NO_DEBUG_OPT
  Serial.println("Initialized ultrasonic.");
#endif
}

unsigned int ReadLeftUS() {
  const unsigned int reading = sonar_l.ping_cm();
#ifdef NO_DEBUG_OPT
#ifdef DEBUG_US
  Serial.print(" Left: " + ((String) reading));
#endif
#endif
  return reading;
}

unsigned int ReadRightUS() {
  const unsigned int reading = sonar_r.ping_cm();
#ifdef NO_DEBUG_OPT
#ifdef DEBUG_US
  Serial.print(" Right: " + ((String) reading));
#endif
#endif
  return reading;
}
/*
unsigned int ReadFrontUS() {
  const unsigned int reading = sonar_f.ping_cm();
#ifdef NO_DEBUG_OPT
#ifdef DEBUG_US
  Serial.print("Front: " + ((String) reading));
#endif
#endif
  return reading;
}
*/
unsigned int ReadBackUS() {
  const unsigned int reading = sonar_b.ping_cm();
#ifdef NO_DEBUG_OPT
#ifdef DEBUG_US
  Serial.println(" Back: " + ((String) reading));
#endif
#endif
  return reading;
}
