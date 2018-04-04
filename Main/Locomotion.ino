// LOW = anti-clockwise
// HIGH = clockwise
#ifdef IS_STRIKER
#define DIR_FL 47
#define SPD_FL 10

#define DIR_FR 45
#define SPD_FR 9

#define DIR_BL 51
#define SPD_BL 12

#define DIR_BR 49
#define SPD_BR 11
#else
#define DIR_FL 51
#define SPD_FL 10

#define DIR_FR 49
#define SPD_FR 11

#define DIR_BL 47
#define SPD_BL 12

#define DIR_BR 45
#define SPD_BR 13
#endif

// Math constants
#define SQRT_2 1.41421356237
#define PI 3.14159265359
#define MAX_SPD 255
#define PI_TO_DEG 180

// Tuning
#define P_GAIN 0.8
#define CMP_BACK_DEG 160
#define CMP_TOL_DEG 5
#define MAX_OFFSET 100

void InitLoc() {
  pinMode(DIR_FL, OUTPUT);
  pinMode(DIR_FR, OUTPUT);
  pinMode(DIR_BL, OUTPUT);
  pinMode(DIR_BR, OUTPUT);

  Serial.println("Initialized locomotion.");
}

// spd: 0f - 1f
// clockwise: clockwise if true
void Spin(float spd, bool clockwise) {
  if (clockwise) {
    digitalWrite(DIR_FL, LOW);
    digitalWrite(DIR_BL, LOW);
    digitalWrite(DIR_FR, LOW);
    digitalWrite(DIR_BR, LOW);
  } else {
    digitalWrite(DIR_FL, HIGH);
    digitalWrite(DIR_BL, HIGH);
    digitalWrite(DIR_FR, HIGH);
    digitalWrite(DIR_BR, HIGH);
  }
  analogWrite(SPD_FL, spd * 255);
  analogWrite(SPD_BR, spd * 255);
  analogWrite(SPD_FR, spd * 255);
  analogWrite(SPD_BL, spd * 255);
}

// spd: 0f - 1f
// dir: 0 - 360 degrees
void Move(float spd, float dir) {
  const float dir_rad = dir / PI_TO_DEG * PI;
  const float cosine = cos(dir_rad);
  const float sine = sin(dir_rad);
  const float fl_br_frac = (cosine + sine) * spd / SQRT_2;
  const float fr_bl_frac = (cosine - sine) * spd / SQRT_2;

  // Rotate to correct to 0 deg bearing
  int fl, br = fl_br_frac * MAX_SPD;
  int fr, bl = fr_bl_frac * MAX_SPD;
  const int compass = ReadCmp();
  const int error = compass > CMP_TOL_DEG ? 360 - compass : compass;
  const int offset = min(MAX_OFFSET, P_GAIN * error);
  if (compass > CMP_TOL_DEG && compass <= CMP_BACK_DEG) {
    br += offset;
    fl -= offset;
    bl -= offset;
    fr += offset;
  } else if (compass < 360 - CMP_TOL_DEG && compass > CMP_BACK_DEG) {
    br -= offset;
    fl += offset;
    fr -= offset;
    bl += offset;
  }

  // Directions
  digitalWrite(DIR_FL, fl > 0 ? LOW : HIGH);
  digitalWrite(DIR_BR, br > 0 ? HIGH : LOW);
  digitalWrite(DIR_FR, fr > 0 ? HIGH : LOW);
  digitalWrite(DIR_BL, bl > 0 ? LOW : HIGH);

#ifdef DEBUG_LOCOMOTION
  Serial.print(" fl: " + ((String) fl) + (fl > 0 ? " low" : " high"));
  Serial.print(" fr: " + ((String) fr) + (fr <= 0 ? " low" : " high"));
  Serial.print(" br: " + ((String) br) + (br <= 0 ? " low" : " high"));
  Serial.println(" bl: " + ((String) bl) + (bl > 0 ? " low" : " high"));
#endif

  fl = min(MAX_SPD, abs(fl));
  br = min(MAX_SPD, abs(br));
  fr = min(MAX_SPD, abs(fr));
  bl = min(MAX_SPD, abs(bl));

  analogWrite(SPD_FL, fr);
  analogWrite(SPD_BR, br);
  analogWrite(SPD_FR, fr);
  analogWrite(SPD_BL, bl);
}
