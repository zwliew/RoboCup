#define GT A15

#define GT_THRES_IN 20

int ReadGate() {
  return analogRead(GT);
}

bool IsBallInGate(int reading) {
  return reading < GT_THRES_IN;
}
