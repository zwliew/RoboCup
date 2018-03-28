#define SLD 50

void InitSld() {
  pinMode(SLD, OUTPUT);

  Serial.println("Initialized solenoid.");
}

void Shoot() {
  digitalWrite(SLD, LOW);
  delay(10);
  digitalWrite(SLD, HIGH);
}
