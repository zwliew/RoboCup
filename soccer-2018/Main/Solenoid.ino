#define SLD 41

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
