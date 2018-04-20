#ifdef IS_STRIKER
#define SLD 41
#else
#define SLD 50
#endif

void InitSld() {
  pinMode(SLD, OUTPUT);
  digitalWrite(SLD, HIGH);

#ifdef NO_DEBUG_OPT
  Serial.println("Initialized solenoid.");
#endif
}

void Shoot() {
  digitalWrite(SLD, LOW);
  delay(10);
  digitalWrite(SLD, HIGH);
}
