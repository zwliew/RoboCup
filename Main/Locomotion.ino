// LOW = anti-clockwise
// HIGH = clockwise
#define DIR_FL 47
#define SPD_FL 10

#define DIR_FR 45
#define SPD_FR 9

#define DIR_BL 51
#define SPD_BL 12

#define DIR_BR 49
#define SPD_BR 11

#define DEBUG_SPD 0.2

void InitLoc() {
  pinMode(DIR_FL, OUTPUT);
  pinMode(DIR_FR, OUTPUT);
  pinMode(DIR_BL, OUTPUT);
  pinMode(DIR_BR, OUTPUT);
}

// spd: 0f - 1f
// clockwise: clockwise if true
void Turn(float spd, bool clockwise) {
  if (clockwise) {
    digitalWrite(DIR_FL, HIGH);
    digitalWrite(DIR_BL, HIGH);
    digitalWrite(DIR_FR, LOW);
    digitalWrite(DIR_BR, LOW);
  } else {
    digitalWrite(DIR_FL, LOW);
    digitalWrite(DIR_BL, LOW);
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
  float dir_rad = dir / 180 * 3.141592;
  float tmp[] = { cos(dir_rad), sin(dir_rad) };
  float y = (tmp[0] - tmp[1]) * spd;
  float x = (tmp[0] + tmp[1]) * spd;

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
  analogWrite(SPD_FL, right);
  analogWrite(SPD_BR, right);
  analogWrite(SPD_FR, left);
  analogWrite(SPD_BL, left);
}
