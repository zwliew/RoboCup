#define SPD_DRIB 2
#define DIR_DRIB 43

void InitDribbler() {
  pinMode(DIR_DRIB, OUTPUT);

  Serial.println("Initialized dribbler.");
}

void Dribble() {
  digitalWrite(DIR_DRIB, LOW);
  analogWrite(SPD_DRIB, 255);
}

void StopDribble() {
  analogWrite(SPD_DRIB, 0);
}
