#ifdef IS_STRKER
#define SLD 41
#else
#define SLD 50
#endif

void InitSld() {
  pinMode(SLD, OUTPUT);

#ifdef NO_DEBUG_OPT
  Serial.println("Initialized solenoid.");
#endif
}

void Shoot() {
  digitalWrite(SLD, LOW);
  delay(10);
  digitalWrite(SLD, HIGH);
}
