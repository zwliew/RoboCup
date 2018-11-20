#define SPD_DRIB 2
#define DIR_DRIB 43

void InitDribbler() {
  pinMode(DIR_DRIB, OUTPUT);

  // Dribble outwards if the bot is the goalie, inwards otherwise.
#ifdef IS_STRIKER
  digitalWrite(DIR_DRIB, LOW);
#else
  digitalWrite(DIR_DRIB, HIGH);
#endif

#ifdef NO_DEBUG_OPT
  Serial.println("Initialized dribbler.");
#endif
}

void Dribble() {
  analogWrite(SPD_DRIB, 100);
}

void StopDribble() {
  analogWrite(SPD_DRIB, 0);
}
