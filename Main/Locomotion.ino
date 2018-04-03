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
  // Calculate speeds
  float dir_rad = dir / 180 * 3.14159265;
  float tmp[] = { cos(dir_rad), sin(dir_rad) };
  float y = (tmp[0] - tmp[1]) * spd / 1.41421356;
  float x = (tmp[0] + tmp[1]) * spd / 1.41421356;

  // Configure directions
  if (x > 0) {
    digitalWrite(DIR_FL, LOW);
    digitalWrite(DIR_BR, HIGH);
  } else if (x < 0) {
    digitalWrite(DIR_FL, HIGH);
    digitalWrite(DIR_BR, LOW);
  }

  if (y > 0) {
    digitalWrite(DIR_FR, HIGH);
    digitalWrite(DIR_BL, LOW);
  } else if (y < 0) {
    digitalWrite(DIR_FR, LOW);
    digitalWrite(DIR_BL, HIGH);
  }

  // Configure speeds
  int right = abs(x) * 255;
  int left = abs(y) * 255;

  int fl, br, fr, bl;
  fl = br = right;
  fr = bl = left;

  int compass = ReadCmp();
  int OFFSET = 80 * (compass > 180 ? 360 - compass : compass) / 180;
  if (compass > 5 && compass < 180) {
    // anti-clockwise
    if (x > 0) {
      br += OFFSET;
      fl -= OFFSET;
    } else {
      br -= OFFSET;
      fl += OFFSET;
    }

    if (y > 0) {
      fr += OFFSET;
      bl -= OFFSET;
    } else {
      fr -= OFFSET;
      bl += OFFSET;
    }
  } else if (compass < 355 && compass > 179) {
    // clockwise
    if (x > 0) {
      br -= OFFSET;
      fl += OFFSET;
    } else {
      br += OFFSET;
      fl -= OFFSET;
    }

    if (y > 0) {
      fr -= OFFSET;
      bl += OFFSET;
    } else {
      fr += OFFSET;
      bl -= OFFSET;
    }
  }

  Serial.print(" fl: ");
  Serial.print(fl);
  Serial.print(" fr: ");
  Serial.print(fr);
  Serial.print(" br: ");
  Serial.print(br);
  Serial.print(" bl: ");
  Serial.println(bl);

  analogWrite(SPD_FL, fl);
  analogWrite(SPD_BR, br);
  analogWrite(SPD_FR, fr);
  analogWrite(SPD_BL, bl);
}
