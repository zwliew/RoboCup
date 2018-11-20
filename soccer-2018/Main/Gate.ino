#define GT A15

#define GT_THRES_IN 985

unsigned int ReadGate() {
  const unsigned int reading = analogRead(GT);
#ifdef DEBUG_GATE
  Serial.println("Gate: " + ((String) reading));
#endif
  return reading;
}

bool IsBallInGate(int reading) {
  return reading < GT_THRES_IN;
}
