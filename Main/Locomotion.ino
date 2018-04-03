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
  float dir_rad = dir / 180 * 3.14159265;
  float tmp[] = { cos(dir_rad), sin(dir_rad) };

  // Speeds
  float fracSpds[] = {
    (tmp[0] + tmp[1]) * spd / 1.41421356,
    (tmp[0] - tmp[1]) * spd / 1.41421356
  };

  // Rotation
  int fl, br, fr, bl;
  fl = br = abs(fracSpds[0]) * 255;
  fr = bl = abs(fracSpds[1]) * 255;
  int compass = ReadCmp();
  int error1 = compass > 180 ? 360 - compass : compass;
  float P_GAIN = 0.8;
/*
  TODO: try again
  int error2 = compass > 180 ? -(360 - compass) : compass;
  float i_gain = 0.0001;
  static float i_mem = 0;
  i_mem += i_gain * error2;
*/
  int offset = P_GAIN * error1;
  if (compass > 5 && compass < 180) {
    br += offset;
    fl -= offset;
    bl -= offset;
    fr += offset;
  } else if (compass < 355 && compass > 179) {
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
  Serial.print(" fl: ");
  Serial.print(fl);
  Serial.print(fl > 0 ? "low" : "high");
  Serial.print(" fr: ");
  Serial.print(fr);
  Serial.print(fr <= 0? "low" : "high");
  Serial.print(" br: ");
  Serial.print(br);
  Serial.print(br <= 0 ? "low" : "high");
  Serial.print(" bl: ");
  Serial.print(bl);
  Serial.println(bl > 0 ? "low" : "high");
#endif

  analogWrite(SPD_FL, abs(fl));
  analogWrite(SPD_BR, abs(br));
  analogWrite(SPD_FR, abs(fr));
  analogWrite(SPD_BL, abs(bl));
}
