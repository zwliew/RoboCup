#define SPD_DRIB 2
#define DIR_DRIB 43

void InitDribbler() {
  pinMode(DIR_DRIB, OUTPUT);
  digitalWrite(DIR_DRIB, LOW);

#ifdef NO_DEBUG_OPT
  Serial.println("Initialized dribbler.");
#endif
}

void Dribble() {
  analogWrite(SPD_DRIB, 200);
}

void StopDribble() {
  analogWrite(SPD_DRIB, 0);
}
